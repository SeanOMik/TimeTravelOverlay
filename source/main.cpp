#define TESLA_INIT_IMPL // If you have more than one file using the tesla header, only define this in the main one
#include <tesla.hpp>    // The Tesla Header

#include <switch.h>

#include "elm_button.hpp"

Result rc;

class TimeTravelGui : public tsl::Gui {
public:
    TimeTravelGui() { }

    // Called when this Gui gets loaded to create the UI
    // Allocate all your elements on the heap. libtesla will make sure to clean them up when not needed anymore
    virtual tsl::elm::Element* createUI() override {
        auto rootFrame = new tsl::elm::OverlayFrame("Time Travel", "v0.1.0-beta");
        auto list = new tsl::elm::List();

        if (R_FAILED(rc)) {
            printf("setInitialize() failed: 0x%x.\n", rc);

            // If we failed to initialize, display the error.
            list->addItem(new tsl::elm::ListItem("Failed to initialize! (0x" + std::to_string(rc) + ")"));
        }

        u64 current_time;
        if (R_SUCCEEDED(rc)) {
            // Set system time
            rc = timeGetCurrentTime(TimeType_Default, &current_time);

            // If we failed to get the time, display the error.
            if (R_FAILED(rc)) {
                printf("timeGetCurrentTime() failed: 0x%x.\n", rc);

                list->addItem(new tsl::elm::ListItem("Failed get current time! (0x" + std::to_string(rc) + ")"));
            } else { // Only add the +1 day button if it got time correctly.
                list->addItem(new Button("+1 Day", [=](u64 keys, Button btn)->bool {
                    if (keys & KEY_A) {
                        u64 day_ahead_time = current_time + 86400; // Add one day (in seconds) to the current time
                        rc = timeSetCurrentTime(TimeType_Default, current_time);

                        if (R_SUCCEEDED(rc)) {
                            list->addItem(new tsl::elm::ListItem("Added 1 day!"));
                        } else {
                            printf("timeSetCurrentTime() failed: 0x%x.\n", rc);

                            // If we failed to set time, display the error.
                            list->addItem(new tsl::elm::ListItem("Failed to set time! (0x" + std::to_string(rc) + ")"));
                        }
                        
                        return true;
                    }

                    return false;
                }), tsl::style::ListItemDefaultHeight);
            }
        }

        rootFrame->setContent(list);

        return rootFrame;
    }
};

class TimeTravelOverlay : public tsl::Overlay {
public:
                                             // libtesla already initialized fs, hid, pl, pmdmnt, hid:sys, set:sys and the SD card
    virtual void initServices() override {  // Called at the start to initialize all services necessary for this Overlay
        consoleInit(NULL);
        timeInitialize();

        rc = setInitialize();
    }

    virtual void exitServices() override { // Called at the end to clean up all services previously initialized
        timeExit();
    }  

    virtual std::unique_ptr<tsl::Gui> loadInitialGui() override {
        return initially<TimeTravelGui>();  // Initial Gui to load. It's possible to pass arguments to it's constructor like this
    }
};

int main(int argc, char **argv) {
    return tsl::loop<TimeTravelOverlay>(argc, argv);
}
