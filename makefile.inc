help:
	@echo "make build:    build static pages in public/ directory"
	@echo "make gh-pages: build static pages and make new commit in gh-pages"
	@echo "make undo:     drop last commit of gh-pages branch"

build-release:
	-rm -rf public-release/
	hugo -d public-release $(HUGO_OPTS)
	cp CNAME public-release/

gh-pages: build-release
	@if ! git rev-parse refs/heads/gh-pages >/dev/null 2>&1 ; then \
		echo "Branch gh-pages not exists, forgot check it out?" >&2; \
		exit 1; \
	fi
	@git add public-release && \
	tree=$$(git write-tree) && \
	newtree=$$(git ls-tree $$tree | grep "public-release$$" | awk '{print $$3;}') && \
	oldtree=$$(git rev-parse refs/heads/gh-pages^{tree}) && \
	if [ "$$oldtree" = "$$newtree" ]; then \
	  echo "HTML is uptodate, branch gh-pages not changed." ; \
	else \
	  commit=$$(git log --format=%B -1 | git commit-tree $$newtree -p refs/heads/gh-pages) && \
	  git update-ref -m "HTML compiled from $$(git rev-parse HEAD)" refs/heads/gh-pages $$commit && \
	  echo "Branch gh-pages changed." ; \
	fi && \
	git rm --cached -r -q public-release && \
	rm -rf public-release

undo:
	@if ! git rev-parse refs/heads/gh-pages^ >/dev/null 2>&1 ; then \
		echo "Branch gh-pages not exists or only one commit, forgot check it out?" >&2; \
		exit 1; \
	fi
	if git update-ref -m "shift from $$(git rev-parse refs/heads/gh-pages)" refs/heads/gh-pages $$(git rev-parse refs/heads/gh-pages^); then \
		echo "gh-pages branch shift to last commit."; \
	fi

.PHONY: help build-release gh-pages undo
