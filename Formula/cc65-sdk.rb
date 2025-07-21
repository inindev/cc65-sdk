class Cc65Sdk < Formula
  desc "Complete cross development package for 65(C)02 systems"
  homepage "https://cc65.github.io/cc65/"
  version "2.20"
  license "Zlib"
  no_autobump! because: :requires_manual_review

  if Hardware::CPU.arm?
    url "https://github.com/inindev/cc65-sdk/releases/download/v2.20/cc65-sdk-2.20-macos-arm64.tar.gz"
    sha256 "b369d857f6f7d587873563f64b007219d06d1738b74285deb3c1682dae33acff"
  else
    url "https://github.com/inindev/cc65-sdk/releases/download/v2.20/cc65-sdk-2 nix86_64.tar.gz"
    sha256 "<insert_x86_64_sha256_here>"
  end

  def install
    prefix.install Dir["cc65-sdk/#{version}/*"]
  end

  def caveats
    <<~EOS
      To use cc65-sdk, add the following to your shell configuration (e.g., ~/.zshrc):
        export CC65_HOME=#{prefix}/share/cc65-sdk
      Then run:
        source ~/.zshrc
      To uninstall later, run:
        brew uninstall cc65-sdk
    EOS
  end

  def test
    (testpath/"foo.c").write "int main (void) { return 0; }"
    system bin/"cl65", "foo.c"
    assert_path_exists testpath/"foo"
  end
end
