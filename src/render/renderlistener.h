#ifndef RENDERLISTENER_H
#define RENDERLISTENER_H


namespace Caelum {


struct RenderEvent
{
    /** Elapsed time in seconds since the last event.
        This gives you time between frame start & frame end,
        and between frame end and next frame start.
        @remarks
            This may not be the elapsed time but the average
            elapsed time between recently fired events.
    */
    float timeSinceLastEvent;
    /** Elapsed time in seconds since the last event of the same type,
        i.e. time for a complete frame.
        @remarks
            This may not be the elapsed time but the average
            elapsed time between recently fired events of the same type.
    */
    float timeSinceLastRender;
};

class RenderListener {
  public:
    RenderListener() {}
    virtual ~RenderListener() {}

    /** Called when a frame is about to begin rendering.
    @remarks
        This event happens before any render targets have begun updating.
        @return
            True to go ahead, false to abort rendering and drop
            out of the rendering loop.
    */
    virtual bool preRenderUpdate(const RenderEvent& evt) = 0;

    /** Called after all render targets have had their rendering commands
        issued, but before render windows have been asked to flip their
        buffers over.
    @remarks
        The usefulness of this event comes from the fact that rendering
        commands are queued for the GPU to process. These can take a little
        while to finish, and so while that is happening the CPU can be doing
        useful things. Once the request to 'flip buffers' happens, the thread
        requesting it will block until the GPU is ready, which can waste CPU
        cycles. Therefore, it is often a good idea to use this callback to
        perform per-frame processing. Of course because the frame's rendering
        commands have already been issued, any changes you make will only
        take effect from the next frame, but in most cases that's not noticeable.
    @return
        True to continue rendering, false to drop out of the rendering loop.
    */
    virtual bool renderingUpdate(const RenderEvent& evt) { return true;}

    /** Called just after a frame has been rendered.
    @remarks
        This event happens after all render targets have been fully updated
        and the buffers switched.
        @return
            True to continue with the next frame, false to drop
            out of the rendering loop.
    */
    virtual bool postRenderUpdate(const RenderEvent& evt) = 0;
};

}

#endif // RENDERLISTENER_H
