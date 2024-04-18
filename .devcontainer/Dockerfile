FROM ubuntu

RUN apt-get update && apt-get install -y \
    sudo curl wget zsh language-pack-en \
    g++-mipsel-linux-gnu \
    gdb-multiarch \
    git make \
    gzip p7zip-full && \
    update-locale LANG=en_US.UTF-8

# Install PsyQ
RUN wget http://psx.arthus.net/sdk/Psy-Q/psyq-4.7-converted-full.7z && \
    7z x psyq-4.7-converted-full.7z -o/var/lib/psyq && \
    chmod -R 777 /var/lib/psyq && \
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
