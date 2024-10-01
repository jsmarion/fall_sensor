#pragma once

#include "esphome/components/button/button.h"
#include "../c1001.h"

namespace esphome
{
    namespace c1001
    {

        class RestartButton : public button::Button, public Parented<C1001Component>
        {
        public:
            RestartButton() = default;

        protected:
            void press_action() override;
        };

    } // namespace c1001
} // namespace esphome