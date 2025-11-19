#pragma once

// AccountBattleView

#include <entt/entt.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/elements.hpp>

#include "fairlanes/context/account_ctx.hpp"

namespace fairlanes::widgets {
using fairlanes::context::AccountCtx;

class AccountBattleView : public ftxui::ComponentBase {
public:
  AccountBattleView(AccountCtx &context);
  ftxui::Element Render() override;

private:
  AccountCtx ctx_;
  ftxui::Component body_;
};

} // namespace fairlanes::widgets
