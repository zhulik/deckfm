FROM archlinux:base AS base

RUN pacman -Syy &&\
    pacman -Syu --noconfirm &&\
    pacman -S --noconfirm qt5-base qt5-quickcontrols2 qt5-wayland qt5-graphicaleffects ttf-droid fontconfig &&\
    yes | pacman -Scc

RUN useradd -m -p $(openssl passwd -1 password) deck

USER deck

RUN mkdir /home/deck/deckfm

WORKDIR /home/deck/deckfm

FROM base as builder

USER root

RUN pacman -Syy &&\
    pacman -Syu --noconfirm &&\
    pacman -S --noconfirm gcc make cmake &&\
    yes | pacman -Scc

USER deck

ADD . .

RUN mkdir build &&\
    cd build &&\
    cmake .. &&\
    make -j$(nproc)

FROM base as runner
ENV QT_QPA_PLATFORM=wayland

COPY --from=builder /home/deck/deckfm ./
