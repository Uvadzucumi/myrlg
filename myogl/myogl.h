#ifndef MYOGL_H_INCLUDED
#define MYOGL_H_INCLUDED

#include <stdio.h>

#include "config.h"
#include "render.h"
#include "events.h"
#include "entity.h"

namespace MyOGL{

    // application class
    class CApplication : public CEvent{
            float FPS;
            double DeltaTime;
            void Events(SDL_Event *Event,  double DeltaTime);  // parse application events
            bool Running;
            bool KEYS[322];  // 322 is the number of SDLK_DOWN events
            char *user_home_dir;
        public:
            CApplication(){ FPS=-1; Running=false; OnRender=NULL; OnLoop=NULL; OnEvent=NULL; user_home_dir=NULL;};
            ~CApplication(){
                if(user_home_dir){
                    delete user_home_dir;
                }
            }
            // create window and render context
            bool Init(int width=800, int height=600, int bpp=32, bool full_screen=false, const char *title=NULL);
            int Run(void); // Start Application
            void Free();   // clear all engine data
            void SetWinCaption(const char *title){ Render->SetWinCaption(title);};
            void SetWinIcon(const char *file_name){ Render->SetWinIcon(file_name);};
            float GetFPS(){ return FPS; }
            // user functions
            void( *OnRender)(double dt);
            void( *OnLoop)(double dt);
            //void( *OnEvent)(SDL_Event *OnEvent);
            void( *OnEvent)(SDL_Event *Event, double DeltaTime);

            //void OnEvent(SDL_Event* Event);
            void OnExit() { Running = false; }
            // key pressed state
            bool IsKeyPressed(unsigned int key_code){
                if(key_code>321) return false;
                return KEYS[key_code];
            }
    };



}

#endif // MYOGL_H_INCLUDED
