# I had to use an older version as I was getting package signature errors with latest
FROM archlinux:base-devel-20220130.0.46058

# A sligtly modified script from https://gitlab.com/popsulfr/steam-deck-tricks#use-podman-to-create-a-steamosarch-development-image
RUN echo 'Server = https://steamdeck-packages.steamos.cloud/archlinux-mirror/$repo/os/$arch' > /etc/pacman.d/mirrorlist &&\
    sed -i 's|^\(\[core\]\)|[jupiter]\nInclude = /etc/pacman.d/mirrorlist\nSigLevel = Never\n\n[holo]\nInclude = /etc/pacman.d/mirrorlist\nSigLevel = Never\n\n\1|' /etc/pacman.conf  &&\
    pacman -Syy  &&\
    pacman-key --init &&\
    pacman-key --populate archlinux  &&\
    pacman -S --noconfirm holo-keyring  &&\
    pacman -Qqn | pacman -S --noconfirm --overwrite='*' -  &&\
    yes | pacman -Scc  &&\
    useradd -m deck  &&\
    echo 'deck ALL=(ALL) NOPASSWD: ALL' > /etc/sudoers.d/deck

RUN pacman -Syy &&\
    pacman -Syu --noconfirm &&\
    pacman -S --noconfirm qt5-base qt5-quickcontrols2 qt5-wayland \
    qt5-graphicaleffects ttf-droid fontconfig git cmake &&\
    yes | pacman -Scc

USER deck
