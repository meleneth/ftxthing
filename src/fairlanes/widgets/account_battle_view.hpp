#pragma once

// AccountBattleView

#include <entt/entt.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/elements.hpp>

#include "fairlanes/context/account_ctx.hpp"

namespace fairlanes::widgets {
using fairlanes::context::EntityCtx;

class AccountBattleView : public ftxui::ComponentBase {
public:
  AccountBattleView(EntityCtx context);
  ftxui::Element Render() override;
  ~AccountBattleView() override = default;

private:
  ftxui::Component body_{nullptr};
  fairlanes::context::EntityCtx ctx_;
};

} // namespace fairlanes::widgets
