#pragma once

#include <string>

extern "C" {
    #include <gst/gst.h>
    #include <gst/rtsp-server/rtsp-server.h>
}

struct ServerInstances{
    gboolean check;
    GMainLoop *loop;
    GstRTSPServer *server;
    GstRTSPMountPoints *mounts;
    GstRTSPMediaFactory *factory;

    ServerInstances():check(gst_init_check(nullptr, nullptr, nullptr)), 
                        loop(g_main_loop_new(NULL, FALSE)),
                        server(gst_rtsp_server_new()),
                        mounts(gst_rtsp_server_get_mount_points(server)),
                        factory(gst_rtsp_media_factory_new ()) {}
};

class Broadcaster{
private:
    ServerInstances instances;
    std::string create_pipeline();
    
public:
    Broadcaster();
    ~Broadcaster();

    int launchServer() const;
};