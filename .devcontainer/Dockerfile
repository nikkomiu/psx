FROM ubuntu:23.10 AS pcsx-redux-build

RUN apt-get update && apt-get install -y \
    build-essential git make pkg-config clang \
    g++ g++-mipsel-linux-gnu cpp-mipsel-linux-gnu \
    binutils-mipsel-linux-gnu libcapstone-dev libfreetype-dev \
    libavcodec-dev libavformat-dev libavutil-dev libcurl4-openssl-dev \
    libglfw3-dev libswresample-dev libuv1-dev zlib1g-dev

WORKDIR /src/pcsx-redux
RUN git clone --recursive https://github.com/grumpycoders/pcsx-redux.git . && \
    git checkout 692929066633f729784e1e5b19f586d7ffec1f3f
RUN make

RUN apt-get install -y imagemagick
RUN make install DESTDIR=/out

FROM ubuntu

# Install zsh and locale (for zsh)
RUN apt-get update && apt-get install -y zsh language-pack-en && update-locale LANG=en_US.UTF-8

# Install dev dependencies
RUN apt-get install -y \
    sudo curl wget unzip gzip p7zip-full \
    build-essential git make cmake \
    g++-mipsel-linux-gnu gdb-multiarch \
    python3

# Install ARMIPS
RUN git clone --recursive https://github.com/Kingcom/armips.git && \
    mkdir -p armips/build && \
    cd armips/build && \
    cmake -DCMAKE_BUILD_TYPE=Release .. && \
    cmake --build . && \
    cp armips /usr/local/bin && \
    cd ../.. && \
    rm -rf armips

# Install nugget
RUN git clone --recursive https://github.com/pcsx-redux/nugget.git /usr/local/nugget && \
    chmod -R 777 /usr/local/nugget

# Install PsyQ
RUN wget http://psx.arthus.net/sdk/Psy-Q/psyq-4.7-converted-full.7z && \
    7z x psyq-4.7-converted-full.7z -o/usr/local/psyq && \
    chmod -R 777 /usr/local/psyq && \
    rm -rf psyq-4.7-converted-full.7z

# Install PCSX
COPY --from=pcsx-redux-build /out /usr/local/
COPY --from=pcsx-redux-build /lib/aarch64-linux-gnu/ /lib/aarch64-linux-gnu/
RUN apt-get install -y mesa-utils libgl1-mesa-glx

# Setup user account
RUN useradd -m coder && \
    usermod -aG sudo coder && \
    echo "%sudo ALL=(ALL:ALL) NOPASSWD: ALL" > /etc/sudoers.d/sudo && \
    chsh -s /bin/zsh coder && \
    git clone --depth=1 https://github.com/powerline/fonts.git ~/.local/share/fonts

USER coder

# Setup Oh My Zsh
RUN sh -c "$(curl -fsSL https://raw.github.com/ohmyzsh/ohmyzsh/master/tools/install.sh)" && \
    sed -i 's/ZSH_THEME="robbyrussell"/ZSH_THEME="agnoster"/g' ~/.zshrc && \
    sed -i 's/plugins=(git)/plugins=(git npm)/g' ~/.zshrc && \
    echo "DISABLE_UPDATE_PROMPT=true" >> ~/.zshrc
