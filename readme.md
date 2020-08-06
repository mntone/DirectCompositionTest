# DirectCompositionTest

[![License](https://img.shields.io/github/license/mntone/DirectCompositionTest.svg?style=flat-square)](https://github.com/mntone/DirectCompositionTest/blob/master/LICENSE.txt)

Windows 8 (NT 6.2) から追加された COM-based な DirectComposition を試したときのやつ。

Windows.UI.Composition という Windows Runtime な DirectComposition ではない。ただベースは同じと思われる。

## 根幹技術

以下は調べ上げた DirectComposition の全体です。

DirectComposition 自体は Windows 8 から問題なく使えるので，独自の UI Framework を作る際には有用です。ただし，OS にべったり依存したインターフェースだったため互換性のサポートをするには相当の熟練プログラマーである必要があると思われます。

- Direct2D Effects (Windows 8 および Windows 7 Platform Update)
  https://docs.microsoft.com/en-us/windows/win32/direct2d/effects-overview
  DirectComposition のエフェクトにも使われている。なぜ COM-based な DirectComposition は生の Effect を受け付ける設計じゃないのかはわからん。
- DirectComposition (Windows 8 から; Apple 系の Core Animation に相当する技術だと思ってもらうと良いです)
  https://docs.microsoft.com/en-us/windows/win32/directcomp/directcomposition-portal
  - IDCompositionDevice
    Windows 8の初期版。拡大縮小などの基本的なもののみ搭載。初期の Metro UI と言われていた頃や当時の IE はこれを使って実装されていると思われる。エフェクトなどは使えない。
    https://docs.microsoft.com/en-us/windows/win32/api/dcomp/nn-dcomp-idcompositiondevice
  - IDCompositionDevice2 と IDCompositionDesktopDevice
    Windows 8.1 でなぜか再構築されているインターフェース。二つに分離されているが，IDCompositionDevice2 が Store apps から使えるわけでもなく何故分けたのかわからない。
    https://docs.microsoft.com/en-us/windows/win32/api/dcomp/nn-dcomp-idcompositiondevice2
    https://docs.microsoft.com/en-us/windows/win32/api/dcomp/nn-dcomp-idcompositiondesktopdevice
  - IDCompositionDevice3
    これも Windows 8.1 で追加されたもの。つまりエフェクト類は Windows 8.1 でも使える。
    https://docs.microsoft.com/en-us/windows/win32/api/dcomp/nn-dcomp-idcompositiondevice3
- Windows.UI.Composition (実質 IDCompositionDevice4)
  https://docs.microsoft.com/en-us/windows/apps/desktop/modernize/visual-layer-in-desktop-apps
  API が大きく刷新された。
  - Compositor
    メインとなるクラス。いわゆる DirectX 系の 〜Device と同じ感覚で良い。初期の Windows 10 から使えるが実質 Windows 10 RS2 (build 15063) から使えるものと考えた方が良い。
    https://docs.microsoft.com/en-us/uwp/api/windows.ui.composition.compositor?view=winrt-19041
    - CreateBackdropBrush
      Windows 10 RS1 (build 14393) で追加
    - CreateHostBackdropBrush
      Windows 10 RS2 (build 15063) で追加。
      WinUI 3 ではこの機能が必須で RS2 以降しかサポートできない。


## LICENSE

[MIT License](https://github.com/mntone/DirectCompositionTest/blob/master/LICENSE.txt)


## Author

mntone
- GitHub: https://github.com/mntone
- Twitter: https://twitter.com/mntone (Japanese)
