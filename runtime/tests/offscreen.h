#ifndef WFE_TEST_OFFSCREEN_H
#define WFE_TEST_OFFSCREEN_H
#include <glad/glad.h>
#include <wfe/types.h>
#include <wfe/pool.h>

typedef struct wfeOffscreen {
    wfeUint fbo, rbuf;
    wfeSize height, width;
} wfeOffscreen;

void wfeOffscreenPrepare(wfeOffscreen *opts) {
    glGenFramebuffers(1, &opts->fbo);
    glGenRenderBuffers(1, &opts->rbuf);
    glBindRenderbuffer(opts->rbuf);

    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, width, height);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, opts->fbo);
    glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, opts->rbuf);

    opts->width = width;
    opts->height = height;

    glClearColor(0.f, 0.f, 0.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, opts->fbo);
}

void wfeOffscreenSnapshot(wfeOffscreen *opts, wfePool *pool, wfeData **data) {
    *data = wfePoolGet(pool, sizeof(wfeUint8) * opts->height * opts->width * 4, wfeAlignOf(wfeUint8));
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glReadPixels(0, 0, width, height, GL_RGBA8, GL_UNSIGNED_INT, *data);
}

void wfeOffscreenTerminate(wfeOffscreen *opts) {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &opts->fbo);
    glDeleteRenderbuffers(1, &opts->rbuf);
}


#endif /* WFE_TEST_OFFSCREEN_H */
