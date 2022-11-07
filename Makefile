.PHONY: build.holo/dist/deckfm/deckfm upload

deckfm.tar.gz: build.holo/dist/deckfm/deckfm
	tar -czf deckfm.tar.gz -C ./build.holo/dist ./deckfm

build.holo/dist/deckfm/deckfm:
	BUILDER_UID="$(shell id -u)" BUILDER_GID="$(shell id -g)" docker-compose up

upload: deckfm.tar.gz
	scp deckfm.tar.gz server.home.zhulik.wtf:/home/zhulik/web/deckfm.tar.gz
