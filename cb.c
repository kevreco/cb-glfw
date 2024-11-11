#define CB_IMPLEMENTATION
#include "cb/cb.h"

static int build_glfw(const char* library_type);

/* Build the static library and shared library for GLFW. */

int main(void)
{
    if (build_glfw(cb_STATIC_LIBRARY) != 0)
    {
        return -1;
    }

    if (build_glfw(cb_SHARED_LIBRARY) != 0)
    {
        return -1;
    }

    return 0;
}

static int build_glfw(const char* library_type)
{
    cb_init();

    cb_project("glfw");

    cb_set(cb_BINARY_TYPE, library_type);

    /* Change output directory of the artifact. */
    cb_set(cb_OUTPUT_DIR, library_type == cb_SHARED_LIBRARY ? ".build/glfw_shared" : ".build/glfw_static");
   
    cb_add_many_vnull(cb_FILES,
        "src/context.c",
        "src/init.c",
        "src/input.c",
        "src/monitor.c",
        "src/platform.c",
        "src/vulkan.c",
        "src/window.c",
        "src/egl_context.c",
        "src/osmesa_context.c",
        "src/null_init.c",
        "src/null_monitor.c",
        "src/null_window.c",
        "src/null_joystick.c",
        NULL
    );

#if _WIN32

    cb_add_many_vnull(cb_FILES,
        "src/win32_module.c",
        "src/win32_time.c",
        "src/win32_thread.c",
        "src/win32_init.c",
        "src/win32_joystick.c",
        "src/win32_monitor.c",
        "src/win32_window.c",
        "src/wgl_context.c",
        NULL);

    cb_add_many_vnull(cb_DEFINES,
        "UNICODE",
        "_GLFW_WIN32",
        NULL); 

    cb_add_many_vnull(cb_LIBRARIES,
        "gdi32",
        "shell32",
        "user32",
        "opengl32",
        NULL);

#else /* LINUX - X11 */

    cb_add(cb_CXFLAGS, "-fPIC");

    cb_add_many_v(cb_FILES,
        "src/posix_module.c",
        "src/posix_time.c",
        "src/posix_thread.c",
        "src/x11_init.c",
        "src/x11_monitor.c",
        "src/x11_window.c",
        "src/xkb_unicode.c",
        "src/glx_context.c",
        "src/linux_joystick.c",
        "src/posix_poll.c"
        );

    cb_add_many_v(cb_DEFINES,
        "_GLFW_X11",
        "_GLFW_GLX",
        "_GLFW_EGL");

    cb_add(cb_LIBRARIES,
        "X11");
#endif

    /* Use default default toochain to build the project. */
    if (!cb_bake())
    {
        return -1;
    }

    cb_destroy();

    return 0;
}