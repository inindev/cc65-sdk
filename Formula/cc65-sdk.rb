class Cc65Sdk < Formula
  desc "Complete cross development package for 65(C)02 systems"
  homepage "https://cc65.github.io/cc65/"
  version "2.20"

  if Hardware::CPU.arm?
    url "https://github.com/inindev/cc65-sdk/releases/download/v2.20/cc65-sdk-2.20-macos-arm64.tar.gz"
    sha256 "7260ad719ef6c0ab7a551e8dd207b6db9665cbd15ab9b1d6683365cdf12b6a82"
  else
    url "https://github.com/inindev/cc65-sdk/releases/download/v2.20/cc65-sdk-2.20-macos-x86_64.tar.gz"
    sha256 "<insert_x86_64_sha256_here>"
  end
  license "Zlib"

  def install
    bin.install Dir["opt/homebrew/bin/*"]
    (share/"cc65-sdk").install Dir["opt/homebrew/share/cc65-sdk/*"]
    (share/"doc/cc65-sdk").install Dir["opt/homebrew/share/doc/cc65-sdk/*"]
  end

  def caveats
    <<~EOS
      To use cc65-sdk, add the following to your shell configuration (e.g., ~/.zshrc):
        export CC65_HOME=/opt/homebrew/share/cc65-sdk
      Then run:
        source ~/.zshrc

      To uninstall later, run:
        brew uninstall cc65-sdk
    EOS
  end
end
