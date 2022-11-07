.PHONY: dist-holo
deckfm.tar.gz: build.holo/dist/deckfm/deckfm
	tar -czf deckfm.tar.gz -C ./build.holo/dist ./deckfm

build.holo/dist/deckfm/deckfm:
	BUILDER_UID="$(id -u)" BUILDER_GID="$(id -g)" docker-compose up
