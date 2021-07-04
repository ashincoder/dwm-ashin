# Maintainer: Ashin Antony <ashinant15@gmail.com>
pkgname=dwm-ashin-git
pkgver=6.2
pkgrel=1
pkgdesc="A heavily-patched and customized build of dwm from Ashin Antony."
arch=(x86_64)
url="https://github.com/ashincoder/dwm-ashin.git"
license=('custom:MIT/X')
groups=()
depends=(libx11 libxinerama nerd-fonts-jetbrains-mono freetype2 libxft-bgra)
makedepends=(make)
checkdepends=()
optdepends=(surf)
provides=(dwm)
conflicts=(dwm)
replaces=()
backup=()
options=()
source=("git+$url")
noextract=()
md5sums=('SKIP')
validpgpkeys=()

pkgver() {
  cd "${_pkgname}"
  printf "6.2.r%s.%s" "$(git rev-list --count HEAD)" "$(git rev-parse --short HEAD)"
}

build() {
  cd dwm-ashin
  make X11INC=/usr/include/X11 X11LIB=/usr/lib/X11 FREETYPEINC=/usr/include/freetype2
}

package() {
  cd dwm-ashin 
  mkdir -p ${pkgdir}/opt/${pkgname}
  cp -rf * ${pkgdir}/opt/${pkgname}
  make PREFIX=/usr DESTDIR="${pkgdir}" install
  install -Dm644 LICENSE "${pkgdir}/usr/share/licenses/dwm-ashin/LICENSE"
  install -Dm644 README.org "${pkgdir}/usr/share/doc/dwm-ashin/README.org"
  install -Dm644 "${srcdir}/dwm-ashin/dwm.desktop" "$pkgdir/usr/share/xsessions/dwm.desktop"
}

