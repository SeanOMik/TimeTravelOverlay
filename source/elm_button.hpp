#pragma once
#include <tesla.hpp>

class Button : public tsl::elm::Element {
public:
    inline Button(u16 x, u16 y, u16 w, u16 h, const std::string &txt, std::function<bool(s64, Button)> click_action) : Element(), m_text(txt), m_onClick(click_action), x(x), y(y), width(w), height(h) { }
    inline Button(const std::string &txt, std::function<bool(s64, Button)> cb) : Element(), m_text(txt), m_onClick(cb), x(this->getX()), y(this->getY()), width(this->getWidth()), height(tsl::style::ListItemDefaultHeight) { }

    inline virtual ~Button() {}

    inline tsl::elm::Element* requestFocus(tsl::elm::Element *oldFocus, tsl::FocusDirection direction) override {
        return this;
    }

    inline virtual void draw(tsl::gfx::Renderer *renderer) override {
        this->renderer = renderer;

        renderer->drawRect(this->getX(), this->getY(), this->getWidth(), 1, a({ 0x5, 0x5, 0x5, 0xF }));
        renderer->drawRect(this->getX(), this->getY() + this->getHeight(), this->getWidth(), 1, a({ 0x5, 0x5, 0x5, 0xF }));

        renderer->drawString(this->m_text.c_str(), false, this->getX() + 20, this->getY() + 45, 23, a({ 0xF, 0xF, 0xF, 0xF }));
    }

    inline void layout(u16 parentX, u16 parentY, u16 parentWidth, u16 parentHeight) override {

    }

    inline virtual bool onClick(u64 keys) override {
       return m_onClick(keys, *this);
    }


    tsl::gfx::Renderer *renderer;
private:
    u16 x;
    u16 y;
    u16 width;
    u16 height;

    std::string m_text;
    std::function<bool(s64, Button)> m_onClick;
};