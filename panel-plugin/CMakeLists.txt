find_package(PkgConfig REQUIRED)

pkg_check_modules(GTK3 REQUIRED IMPORTED_TARGET gtk+-3.0>=3.22)
pkg_check_modules(libxfce4panel REQUIRED IMPORTED_TARGET libxfce4panel-2.0>=4.11)
pkg_check_modules(libxfce4ui REQUIRED IMPORTED_TARGET libxfce4ui-2>=4.11)
pkg_check_modules(libxfce4util REQUIRED IMPORTED_TARGET libxfce4util-1.0>=4.11)

add_library(dmb SHARED
        dmb.c
        dmb_dialogs.c
        dmb_configure.c
        dmb_renew.c)

target_link_libraries(dmb
        PkgConfig::GTK3
        PkgConfig::libxfce4panel
        PkgConfig::libxfce4ui
        PkgConfig::libxfce4util
        )

target_compile_definitions(dmb PRIVATE
        GETTEXT_PACKAGE="xfce4-dmbtimer-plugin"
        PACKAGE_NAME="xfce4-dmbtimer-plugin"
        PACKAGE_VERSION="${dmb_version}"
        G_LOG_DOMAIN="dmb"
        GSEAL_ENABLE
        GTK_MULTIHEAD_SAFE
        )

target_compile_options(dmb PRIVATE -Werror)

install(TARGETS dmb LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/xfce4/panel/plugins)
install(FILES dmbcounter.desktop DESTINATION ${CMAKE_INSTALL_DATADIR}/xfce4/panel/plugins)