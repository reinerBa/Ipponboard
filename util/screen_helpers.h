#ifndef X11_HELPERS_H
#define X11_HELPERS_H

#ifdef __linux__
#include <QApplication>
#include <QDesktopWidget>
#include <QList>
#include <QRect>

#include <QTextStream>
#include<X11/Xlib.h>
#include<X11/extensions/Xrandr.h>

class ScreenHelpers
{
public:
    ScreenHelpers()
    {
        init();
    }

    static ScreenHelpers* getInstance()
    {
        if(s_pInstance==NULL)
            s_pInstance = new ScreenHelpers();

        return s_pInstance;
    }

    void init()
    {
        m_screenResolutionsList.clear();

#ifdef __linux__
        // if the QT function doesnt work under Linux, try a X11 solution
        if (QApplication::desktop()->numScreens() < 2)
        {
            Display *display = XOpenDisplay(NULL);
            Window root = RootWindow(display, 0);
            XRRScreenResources* screenr = XRRGetScreenResources(display, root);
            int output = screenr->noutput;
            for (int i = 0; i < output; ++i)
            {
                XRROutputInfo* out_info = XRRGetOutputInfo(display, screenr, screenr->outputs[i]);
                if (NULL != out_info && out_info->connection == RR_Connected)
                {
                    XRRCrtcInfo* crt_info = XRRGetCrtcInfo(display, screenr, out_info->crtc);

                    m_screenResolutionsList.append(QRect(crt_info->x, crt_info->y, crt_info->width, crt_info->height));

                    XRRFreeCrtcInfo(crt_info);
                }
            }
        }
#endif
    }

    const int numScreens()
    {
        if (m_screenResolutionsList.size() > 1)
            return   m_screenResolutionsList.size();
        else
            return QApplication::desktop()->numScreens();
    }

    const QRect getScreenGeometry(int screen_no=-1)
    {
        if (m_screenResolutionsList.size() > 1)
        {
            if (screen_no == -1)
                return m_screenResolutionsList.first();
            else
                return m_screenResolutionsList.at(screen_no);
        }
        else
            return QApplication::desktop()->availableGeometry(screen_no);
    }

protected:
    inline static ScreenHelpers* s_pInstance = NULL;

private:
    QList<QRect> m_screenResolutionsList;
};
#endif
#endif // X11_HELPERS_H

