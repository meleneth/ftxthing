// registry_facade.hpp
#pragma once
#include <entt/entt.hpp>

namespace fairlanes::ecs {

// CRTP mixin: assumes Derived has ctx_.reg_ pointing at an entt::registry
template <typename Derived> struct RegistryFacade {
  entt::registry &registry() {
    return *static_cast<Derived *>(this)->ctx_.reg_;
  }

  const entt::registry &registry() const {
    return *static_cast<const Derived *>(this)->ctx_.reg_;
  }

  template <typename T> T &get(entt::entity e) {
    return registry().template get<T>(e);
  }

  template <typename Component, typename... Args>
  Component &emplace(entt::entity e, Args &&...args) {
    return registry().template emplace<Component>(e,
                                                  std::forward<Args>(args)...);
  }

  // easy to add more:
  template <typename... Components> auto view() {
    return registry().template view<Components...>();
  }
};

} // namespace fairlanes::ecs

// Drop this into the base list of any type that has ctx_.reg_
#define BE_REGISTRY(TypeName) public ::fairlanes::ecs::RegistryFacade<TypeName>
