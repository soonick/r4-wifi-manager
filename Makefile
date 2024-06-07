image:
	@docker build -f dockerfiles/dev -t r4-wifi-manager-dev-image .
.PHONY: image

cmake: image
	@docker run -it --rm \
		-w /r4-wifi-manager/build/ \
		-v $(PWD)/BoardOptions.cmake:/r4-wifi-manager/BoardOptions.cmake \
		-v $(PWD)/CMakeLists.txt:/r4-wifi-manager/CMakeLists.txt \
		-v $(PWD)/build:/r4-wifi-manager/build \
		-v $(PWD)/deps:/r4-wifi-manager/deps \
		-v $(PWD)/include:/r4-wifi-manager/include \
		-v $(PWD)/src:/r4-wifi-manager/src \
		r4-wifi-manager-dev-image \
		sh -c "\
			cmake \
				-D ARDUINO_BOARD_OPTIONS_FILE=/r4-wifi-manager/BoardOptions.cmake \
				-D ARDUINO_ENABLE_PACKAGE_MANAGER=ON \
				-D ARDUINO_INSTALL_PATH=/root/.arduino15 \
				-D ARDUINO_PLATFORM=arduino.renesas_uno \
				-D CMAKE_EXPORT_COMPILE_COMMANDS=1 \
				-D CMAKE_TOOLCHAIN_FILE=Arduino-CMake-Toolchain/Arduino-toolchain.cmake .. && \
			rm -f /r4-wifi-manager/compile_commands.json \
		"
.PHONY: cmake

build: cmake
	@docker run -it --rm \
		-w /r4-wifi-manager/build/ \
		-v $(PWD)/BoardOptions.cmake:/r4-wifi-manager/BoardOptions.cmake \
		-v $(PWD)/CMakeLists.txt:/r4-wifi-manager/CMakeLists.txt \
		-v $(PWD)/build:/r4-wifi-manager/build \
		-v $(PWD)/deps:/r4-wifi-manager/deps \
		-v $(PWD)/include:/r4-wifi-manager/include \
		-v $(PWD)/src:/r4-wifi-manager/src \
		r4-wifi-manager-dev-image \
		sh -c "\
			make \
		"
.PHONY: build

ssh:
	@docker run --rm -it \
		-v $(PWD)/deps:/r4-wifi-manager/deps \
		-v $(PWD)/include:/r4-wifi-manager/include \
		-v $(PWD)/src/:/r4-wifi-manager/src \
		-v $(PWD)/test/:/r4-wifi-manager/test \
		-w /r4-wifi-manager/ r4-wifi-manager-dev-image bash
.PHONY: ssh

format:
	@docker run --rm -it \
		-v $(PWD)/deps:/r4-wifi-manager/deps \
		-v $(PWD)/include:/r4-wifi-manager/include \
		-v $(PWD)/src/:/r4-wifi-manager/src \
		-v $(PWD)/test/:/r4-wifi-manager/test \
		-w /r4-wifi-manager/ \
		r4-wifi-manager-dev-image \
		sh -c "find src/ test/src/ test/include/ -iname '*.h' -o -iname '*.cpp' | xargs clang-format --style=Chromium -i"
.PHONY: format

check-format:
	@docker run --rm -it \
		-v $(PWD)/deps:/r4-wifi-manager/deps \
		-v $(PWD)/include:/r4-wifi-manager/include \
		-v $(PWD)/src/:/r4-wifi-manager/src \
		-v $(PWD)/test/:/r4-wifi-manager/test \
		-w /r4-wifi-manager/ \
		r4-wifi-manager-dev-image \
		sh -c "find src/ test/src/ test/include/ -iname '*.h' -o -iname '*.cpp' | xargs clang-format --style=Chromium --dry-run -Werror"
.PHONY: check-format

cmake_test: image
	@docker run -it --rm \
		-w /r4-wifi-manager/test/build/ \
		-v $(PWD)/deps:/r4-wifi-manager/deps \
		-v $(PWD)/include:/r4-wifi-manager/include \
		-v $(PWD)/src:/r4-wifi-manager/src \
		-v $(PWD)/test/CMakeLists.txt:/r4-wifi-manager/test/CMakeLists.txt \
		-v $(PWD)/test/build:/r4-wifi-manager/test/build \
		-v $(PWD)/test/external:/r4-wifi-manager/test/external \
		-v $(PWD)/test/include:/r4-wifi-manager/test/include \
		-v $(PWD)/test/src:/r4-wifi-manager/test/src \
		r4-wifi-manager-dev-image \
		sh -c " \
			cmake -D CMAKE_EXPORT_COMPILE_COMMANDS=1 .. && \
			rm -f /r4-wifi-manager/test/compile_commands.json \
		"
.PHONY: cmake_test

test: cmake_test
	@docker run -it --rm \
		-v $(PWD)/deps:/r4-wifi-manager/deps \
		-v $(PWD)/include:/r4-wifi-manager/include \
		-v $(PWD)/src:/r4-wifi-manager/src \
		-v $(PWD)/test/CMakeLists.txt:/r4-wifi-manager/test/CMakeLists.txt \
		-v $(PWD)/test/build:/r4-wifi-manager/test/build \
		-v $(PWD)/test/external:/r4-wifi-manager/test/external \
		-v $(PWD)/test/include:/r4-wifi-manager/test/include \
		-v $(PWD)/test/src:/r4-wifi-manager/test/src \
		-w /r4-wifi-manager/test/build/ \
		r4-wifi-manager-dev-image \
		sh -c " \
			make && \
			./test \
		"
.PHONY: test

verify: build check-format test
.PHONY: verify

# Starts a container with a neovim development environment ready to use
vim: cmake cmake_test
	@docker build -f dockerfiles/dev-vim -t smart-fluid-flow-meter-r4-wifi-manager-dev-vim-image .
	@docker run --rm -it \
		-v $(PWD)/CMakeLists.txt:/r4-wifi-manager/CMakeLists.txt \
		-v $(PWD)/Makefile:/r4-wifi-manager/Makefile \
		-v $(PWD)/README.md:/r4-wifi-manager/README.md \
		-v $(PWD)/assets:/r4-wifi-manager/assets \
		-v $(PWD)/build:/r4-wifi-manager/build \
		-v $(PWD)/deps:/r4-wifi-manager/deps \
		-v $(PWD)/include:/r4-wifi-manager/include \
		-v $(PWD)/dev-environments/vim/tmp:/root/.local/share/nvim \
		-v $(PWD)/dockerfiles:/r4-wifi-manager/dockerfiles \
		-v $(PWD)/src:/r4-wifi-manager/src \
		-v $(PWD)/test:/r4-wifi-manager/test \
		-w /r4-wifi-manager/ \
		smart-fluid-flow-meter-r4-wifi-manager-dev-vim-image \
		sh -c " \
			rm -f /r4-wifi-manager/test/compile_commands.json && \
			ln -s /r4-wifi-manager/build/compile_commands.json /r4-wifi-manager/ && \
			ln -s /r4-wifi-manager/test/build/compile_commands.json /r4-wifi-manager/test/ && \
			nvim \
		"
.PHONY: vim
