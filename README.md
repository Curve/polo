<div align="center">
    <img src="assets/logo.svg" height="312">
</div>

<br/>

<p align="center">
    A C++23 polymorphic value library
</p>

<hr/>

## 👋 Introduction

_Polo_ is a small C++23 library that implements a container for polymorphic types which preserves value-semantics

## 📦 Installation

* Using [CPM](https://github.com/cpm-cmake/CPM.cmake)
  ```cmake
  CPMFindPackage(
    NAME           polo
    VERSION        1.0.0
    GIT_REPOSITORY "https://github.com/Curve/polo"
  )
  ```

* Using FetchContent
  ```cmake
  include(FetchContent)

  FetchContent_Declare(polo GIT_REPOSITORY "https://github.com/Curve/polo" GIT_TAG v1.0.0)
  FetchContent_MakeAvailable(polo)

  target_link_libraries(<target> cr::polo)
  ```

