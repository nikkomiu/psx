FROM ubuntu

RUN apt-get update && apt-get install -y \
    sudo curl wget zsh language-pack-en \
    build-essential git make cmake unzip fuse \
    g++-mipsel-linux-gnu gdb-multiarch \
    gzip p7zip-full python3 && \
    update-locale LANG=en_US.UTF-8

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
