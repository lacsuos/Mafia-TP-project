#include "webcam.h"

Broadcaster::Broadcaster()
{}

Broadcaster::~Broadcaster(){
    g_object_unref(instances.server);
    g_object_unref(instances.loop);
    g_object_unref(instances.mounts);
    g_object_unref(instances.factory);
    gst_deinit();
}


std::string Broadcaster::bussCallback() const {}
int Broadcaster::launchServer() const{
    gst_rtsp_media_factory_set_launch (instances.factory,
                                        "( autovideosrc ! x264enc cabac=false tune=zerolatency ! rtph264pay name=pay0 pt=96 ! udpsink)");

    gst_rtsp_media_factory_set_shared (instances.factory, TRUE);

    gst_rtsp_mount_points_add_factory (instances.mounts, "/webcam", instances.factory);
    gst_rtsp_server_attach (instances.server, NULL);
    g_main_loop_run (instances.loop);
    return 0;
}