#include "audio/audiodecoder.h"

#include "core/logmanager.h"

#include <stdlib.h>
#include <vector>

#include <AL/al.h>
#include <AL/alc.h>

//#include <SDL/SDL_audio.h>
#include <vorbis/vorbisfile.h>
#include <vorbis/codec.h>

#define BUFFER_SIZE     32768       // 32 KB buffers

using namespace Caelum;
using namespace std;

/*void Loader::loadWAV(const char* path, ALuint &buffer)
{
    // Load WAV song
    Uint32 size;
    Uint8 *data;
    SDL_AudioSpec wav_spec;
    SDL_LoadWAV( path, &wav_spec, &data, &size);
    // Load buffer here
    alBufferData(buffer, AL_FORMAT_STEREO16, data, size, wav_spec.freq);
    // Free WAV
    SDL_FreeWAV(data);
}*/

void AudioDecoder::old_loadOgg(const String &path, AudioBuffer *buffer) {
    FILE *f;
    f = fopen(path.c_str(), "rb");

    OggVorbis_File oggFile;
    if (ov_open(f, &oggFile, NULL, 0) == 0) {
        vorbis_info *pInfo;
        pInfo = ov_info(&oggFile, -1);
        // Chanels & Format        ( always use 16-bit samples )
        ALenum format;
        (pInfo->channels == 1)? format = AL_FORMAT_MONO16 : format = AL_FORMAT_STEREO16;
        // Frequency
        ALsizei freq = pInfo->rate;

        int endian = 0;                         // 0 for Little-Endian, 1 for Big-Endian
        int bitStream;
        long bytes;
        char array[BUFFER_SIZE];                // Local fixed size array
        vector<char> bufferData;
        do {
            // Read up to a buffer's worth of decoded sound data
            bytes = ov_read(&oggFile, array, BUFFER_SIZE, endian, 2, 1, &bitStream);
            if (bytes < 0) {
                ov_clear(&oggFile);
                exit(-1);
            }
            // Append to end of buffer
            bufferData.insert(bufferData.end(), array, array + bytes);
        } while (bytes > 0);

        // Turn into al buffer
        alBufferData(*(buffer->buffer()), format, &bufferData[0], static_cast<ALsizei>(bufferData.size()), freq);

        // Clean up!
        ov_clear(&oggFile);
    }
}

void AudioDecoder::loadOgg(const String &path, AudioBuffer *buffer) {
    FILE *f;
    f = fopen(path.c_str(), "rb");

    OggVorbis_File oggFile;
    if (ov_open_callbacks(f, &oggFile, NULL, 0, OV_CALLBACKS_NOCLOSE) < 0) {
        LogManager::getSingleton().logMessage("CODEC: Input does not appear to be an Ogg bitstream");
        return;
    }

    // File info - Chanels & Format ( always use 16-bit samples ) + Frequency
    LogManager::getSingleton().logMessage("CODEC: Getting ogg info");
    vorbis_info *pInfo;
    pInfo = ov_info(&oggFile, -1);
    ALenum format;
    format = (pInfo->channels == 1)? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;

    ALsizei freq = pInfo->rate;

    //(long)ov_pcm_total(&oggFile, -1);

    // Actual decoding
    LogManager::getSingleton().logMessage("CODEC: decoding "+path);
    int endian = 0;            // 0 for Little-Endian, 1 for Big-Endian
    int bitStream;
    long bytes;
    char array[BUFFER_SIZE];   // Local fixed size array
    vector<char> bufferData;

    do {
        // Read up to a buffer's worth of decoded sound data
        bytes = ov_read(&oggFile, array, sizeof(array), endian, 2, 1, &bitStream);
        //bytes = ov_read(&oggFile, array, BUFFER_SIZE, endian, 2, 1, &bitStream);
        if (bytes < 0) {
            ov_clear(&oggFile);
            exit(-1);
        }
        // Append to end of buffer
        bufferData.insert(bufferData.end(), array, array + bytes);
    } while (bytes > 0);

    // Turn into al buffer
    alBufferData(*(buffer->buffer()), format, &bufferData[0], static_cast<ALsizei>(bufferData.size()), freq);

    // Clean up!
    ov_clear(&oggFile);
}

//@} load Song
/*ALsizei size,freq;
ALenum 	format;
ALvoid 	*data;
alutLoadWAVFile("../a.wav",&format,&data,&size,&freq);
alBufferData(buffer[0],format,data,size,freq);
alutUnloadWAV(format,data,size,freq);*/
