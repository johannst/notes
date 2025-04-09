// Populate the sidebar
//
// This is a script, and not included directly in the page, to control the total size of the book.
// The TOC contains an entry for each page, so if each page includes a copy of the TOC,
// the total size of the page becomes O(n**2).
class MDBookSidebarScrollbox extends HTMLElement {
    constructor() {
        super();
    }
    connectedCallback() {
        this.innerHTML = '<ol class="chapter"><li class="chapter-item expanded affix "><a href="intro.html">Introduction</a></li><li class="chapter-item expanded "><a href="shells/index.html"><strong aria-hidden="true">1.</strong> Shells</a></li><li><ol class="section"><li class="chapter-item expanded "><a href="shells/zsh.html"><strong aria-hidden="true">1.1.</strong> zsh</a></li><li class="chapter-item expanded "><a href="shells/bash.html"><strong aria-hidden="true">1.2.</strong> bash</a></li><li class="chapter-item expanded "><a href="shells/fish.html"><strong aria-hidden="true">1.3.</strong> fish</a></li></ol></li><li class="chapter-item expanded "><a href="cli/index.html"><strong aria-hidden="true">2.</strong> CLI foo</a></li><li><ol class="section"><li class="chapter-item expanded "><a href="cli/awk.html"><strong aria-hidden="true">2.1.</strong> awk</a></li><li class="chapter-item expanded "><a href="cli/cut.html"><strong aria-hidden="true">2.2.</strong> cut</a></li><li class="chapter-item expanded "><a href="cli/sed.html"><strong aria-hidden="true">2.3.</strong> sed</a></li><li class="chapter-item expanded "><a href="cli/column.html"><strong aria-hidden="true">2.4.</strong> column</a></li><li class="chapter-item expanded "><a href="cli/sort.html"><strong aria-hidden="true">2.5.</strong> sort</a></li><li class="chapter-item expanded "><a href="cli/tr.html"><strong aria-hidden="true">2.6.</strong> tr</a></li><li class="chapter-item expanded "><a href="cli/tac.html"><strong aria-hidden="true">2.7.</strong> tac</a></li><li class="chapter-item expanded "><a href="cli/rev.html"><strong aria-hidden="true">2.8.</strong> rev</a></li><li class="chapter-item expanded "><a href="cli/paste.html"><strong aria-hidden="true">2.9.</strong> paste</a></li><li class="chapter-item expanded "><a href="cli/xargs.html"><strong aria-hidden="true">2.10.</strong> xargs</a></li><li class="chapter-item expanded "><a href="cli/grep.html"><strong aria-hidden="true">2.11.</strong> grep</a></li><li class="chapter-item expanded "><a href="cli/find.html"><strong aria-hidden="true">2.12.</strong> find</a></li></ol></li><li class="chapter-item expanded "><a href="tools/index.html"><strong aria-hidden="true">3.</strong> Tools</a></li><li><ol class="section"><li class="chapter-item expanded "><a href="tools/tmux.html"><strong aria-hidden="true">3.1.</strong> tmux</a></li><li class="chapter-item expanded "><a href="tools/screen.html"><strong aria-hidden="true">3.2.</strong> screen</a></li><li class="chapter-item expanded "><a href="tools/emacs.html"><strong aria-hidden="true">3.3.</strong> emacs</a></li><li class="chapter-item expanded "><a href="tools/gpg.html"><strong aria-hidden="true">3.4.</strong> gpg</a></li><li class="chapter-item expanded "><a href="tools/radare2.html"><strong aria-hidden="true">3.5.</strong> radare2</a></li><li class="chapter-item expanded "><a href="tools/qemu.html"><strong aria-hidden="true">3.6.</strong> qemu</a></li><li class="chapter-item expanded "><a href="tools/pacman.html"><strong aria-hidden="true">3.7.</strong> pacman</a></li><li class="chapter-item expanded "><a href="tools/dot.html"><strong aria-hidden="true">3.8.</strong> dot</a></li><li class="chapter-item expanded "><a href="tools/ffmpeg.html"><strong aria-hidden="true">3.9.</strong> ffmpeg</a></li><li class="chapter-item expanded "><a href="tools/gnuplot.html"><strong aria-hidden="true">3.10.</strong> gnuplot</a></li><li class="chapter-item expanded "><a href="tools/restic.html"><strong aria-hidden="true">3.11.</strong> restic</a></li><li class="chapter-item expanded "><a href="tools/qrencode.html"><strong aria-hidden="true">3.12.</strong> qrencode</a></li></ol></li><li class="chapter-item expanded "><a href="process/index.html"><strong aria-hidden="true">4.</strong> Process management &amp; inspection</a></li><li><ol class="section"><li class="chapter-item expanded "><a href="process/lsof.html"><strong aria-hidden="true">4.1.</strong> lsof</a></li><li class="chapter-item expanded "><a href="process/pidstat.html"><strong aria-hidden="true">4.2.</strong> pidstat</a></li><li class="chapter-item expanded "><a href="process/pgrep.html"><strong aria-hidden="true">4.3.</strong> pgrep</a></li><li class="chapter-item expanded "><a href="process/ps.html"><strong aria-hidden="true">4.4.</strong> ps</a></li><li class="chapter-item expanded "><a href="process/pmap.html"><strong aria-hidden="true">4.5.</strong> pmap</a></li><li class="chapter-item expanded "><a href="process/pstack.html"><strong aria-hidden="true">4.6.</strong> pstack</a></li><li class="chapter-item expanded "><a href="process/taskset.html"><strong aria-hidden="true">4.7.</strong> taskset</a></li><li class="chapter-item expanded "><a href="process/nice.html"><strong aria-hidden="true">4.8.</strong> nice</a></li></ol></li><li class="chapter-item expanded "><a href="trace_profile/index.html"><strong aria-hidden="true">5.</strong> Trace and Profile</a></li><li><ol class="section"><li class="chapter-item expanded "><a href="trace_profile/time.html"><strong aria-hidden="true">5.1.</strong> time</a></li><li class="chapter-item expanded "><a href="trace_profile/strace.html"><strong aria-hidden="true">5.2.</strong> strace</a></li><li class="chapter-item expanded "><a href="trace_profile/ltrace.html"><strong aria-hidden="true">5.3.</strong> ltrace</a></li><li class="chapter-item expanded "><a href="trace_profile/perf.html"><strong aria-hidden="true">5.4.</strong> perf</a></li><li class="chapter-item expanded "><a href="trace_profile/oprofile.html"><strong aria-hidden="true">5.5.</strong> OProfile</a></li><li class="chapter-item expanded "><a href="trace_profile/callgrind.html"><strong aria-hidden="true">5.6.</strong> callgrind</a></li><li class="chapter-item expanded "><a href="trace_profile/valgrind.html"><strong aria-hidden="true">5.7.</strong> valgrind</a></li><li class="chapter-item expanded "><a href="trace_profile/vtune.html"><strong aria-hidden="true">5.8.</strong> vtune</a></li><li class="chapter-item expanded "><a href="trace_profile/tracy.html"><strong aria-hidden="true">5.9.</strong> tracy</a></li></ol></li><li class="chapter-item expanded "><a href="debug/index.html"><strong aria-hidden="true">6.</strong> Debug</a></li><li><ol class="section"><li class="chapter-item expanded "><a href="debug/gdb.html"><strong aria-hidden="true">6.1.</strong> gdb</a></li><li class="chapter-item expanded "><a href="debug/gdbserver.html"><strong aria-hidden="true">6.2.</strong> gdbserver</a></li></ol></li><li class="chapter-item expanded "><a href="binary/index.html"><strong aria-hidden="true">7.</strong> Binary</a></li><li><ol class="section"><li class="chapter-item expanded "><a href="binary/od.html"><strong aria-hidden="true">7.1.</strong> od</a></li><li class="chapter-item expanded "><a href="binary/xxd.html"><strong aria-hidden="true">7.2.</strong> xxd</a></li><li class="chapter-item expanded "><a href="binary/readelf.html"><strong aria-hidden="true">7.3.</strong> readelf</a></li><li class="chapter-item expanded "><a href="binary/objdump.html"><strong aria-hidden="true">7.4.</strong> objdump</a></li><li class="chapter-item expanded "><a href="binary/nm.html"><strong aria-hidden="true">7.5.</strong> nm</a></li></ol></li><li class="chapter-item expanded "><a href="development/index.html"><strong aria-hidden="true">8.</strong> Development</a></li><li><ol class="section"><li class="chapter-item expanded "><a href="development/c++filt.html"><strong aria-hidden="true">8.1.</strong> c++filt</a></li><li class="chapter-item expanded "><a href="development/c++.html"><strong aria-hidden="true">8.2.</strong> c++</a></li><li class="chapter-item expanded "><a href="development/glibc.html"><strong aria-hidden="true">8.3.</strong> glibc</a></li><li class="chapter-item expanded "><a href="development/gcc.html"><strong aria-hidden="true">8.4.</strong> gcc</a></li><li class="chapter-item expanded "><a href="development/gas.html"><strong aria-hidden="true">8.5.</strong> gas</a></li><li class="chapter-item expanded "><a href="development/ld.html"><strong aria-hidden="true">8.6.</strong> ld</a></li><li class="chapter-item expanded "><a href="development/git.html"><strong aria-hidden="true">8.7.</strong> git</a></li><li class="chapter-item expanded "><a href="development/cmake.html"><strong aria-hidden="true">8.8.</strong> cmake</a></li><li class="chapter-item expanded "><a href="development/make.html"><strong aria-hidden="true">8.9.</strong> make</a></li><li class="chapter-item expanded "><a href="development/ld.so.html"><strong aria-hidden="true">8.10.</strong> ld.so</a></li><li class="chapter-item expanded "><a href="development/symbolver.html"><strong aria-hidden="true">8.11.</strong> symbol versioning</a></li><li class="chapter-item expanded "><a href="development/python.html"><strong aria-hidden="true">8.12.</strong> python</a></li><li class="chapter-item expanded "><a href="development/gcov.html"><strong aria-hidden="true">8.13.</strong> gcov</a></li><li class="chapter-item expanded "><a href="development/pgo.html"><strong aria-hidden="true">8.14.</strong> pgo</a></li></ol></li><li class="chapter-item expanded "><a href="linux/index.html"><strong aria-hidden="true">9.</strong> Linux</a></li><li><ol class="section"><li class="chapter-item expanded "><a href="linux/systemd.html"><strong aria-hidden="true">9.1.</strong> systemd</a></li><li class="chapter-item expanded "><a href="linux/coredump.html"><strong aria-hidden="true">9.2.</strong> coredump</a></li><li class="chapter-item expanded "><a href="linux/ptrace_scope.html"><strong aria-hidden="true">9.3.</strong> ptrace_scope</a></li><li class="chapter-item expanded "><a href="linux/cryptsetup.html"><strong aria-hidden="true">9.4.</strong> cryptsetup</a></li><li class="chapter-item expanded "><a href="linux/swap.html"><strong aria-hidden="true">9.5.</strong> swap</a></li><li class="chapter-item expanded "><a href="linux/input.html"><strong aria-hidden="true">9.6.</strong> input</a></li><li class="chapter-item expanded "><a href="linux/acl.html"><strong aria-hidden="true">9.7.</strong> acl</a></li><li class="chapter-item expanded "><a href="linux/zfs.html"><strong aria-hidden="true">9.8.</strong> zfs</a></li><li class="chapter-item expanded "><a href="linux/cpufreq.html"><strong aria-hidden="true">9.9.</strong> cpufreq</a></li><li class="chapter-item expanded "><a href="linux/cups.html"><strong aria-hidden="true">9.10.</strong> cups</a></li></ol></li><li class="chapter-item expanded "><a href="network/index.html"><strong aria-hidden="true">10.</strong> Network</a></li><li><ol class="section"><li class="chapter-item expanded "><a href="network/ssh.html"><strong aria-hidden="true">10.1.</strong> ssh</a></li><li class="chapter-item expanded "><a href="network/ss.html"><strong aria-hidden="true">10.2.</strong> ss</a></li><li class="chapter-item expanded "><a href="network/tcpdump.html"><strong aria-hidden="true">10.3.</strong> tcpdump</a></li><li class="chapter-item expanded "><a href="network/tshark.html"><strong aria-hidden="true">10.4.</strong> tshark</a></li><li class="chapter-item expanded "><a href="network/firewall-cmd.html"><strong aria-hidden="true">10.5.</strong> firewall-cmd</a></li><li class="chapter-item expanded "><a href="network/nftables.html"><strong aria-hidden="true">10.6.</strong> nftables</a></li></ol></li><li class="chapter-item expanded "><a href="web/index.html"><strong aria-hidden="true">11.</strong> Web</a></li><li><ol class="section"><li class="chapter-item expanded "><a href="web/html.html"><strong aria-hidden="true">11.1.</strong> html</a></li><li class="chapter-item expanded "><a href="web/css.html"><strong aria-hidden="true">11.2.</strong> css</a></li><li class="chapter-item expanded "><a href="web/chartjs.html"><strong aria-hidden="true">11.3.</strong> chartjs</a></li><li class="chapter-item expanded "><a href="web/plotly.html"><strong aria-hidden="true">11.4.</strong> plotly</a></li></ol></li><li class="chapter-item expanded "><a href="arch/index.html"><strong aria-hidden="true">12.</strong> Arch</a></li><li><ol class="section"><li class="chapter-item expanded "><a href="arch/cache.html"><strong aria-hidden="true">12.1.</strong> cache</a></li><li class="chapter-item expanded "><a href="arch/x86_64.html"><strong aria-hidden="true">12.2.</strong> x86_64</a></li><li class="chapter-item expanded "><a href="arch/armv8.html"><strong aria-hidden="true">12.3.</strong> armv8</a></li><li class="chapter-item expanded "><a href="arch/arm64.html"><strong aria-hidden="true">12.4.</strong> arm64</a></li><li class="chapter-item expanded "><a href="arch/armv7.html"><strong aria-hidden="true">12.5.</strong> armv7</a></li><li class="chapter-item expanded "><a href="arch/riscv.html"><strong aria-hidden="true">12.6.</strong> riscv</a></li></ol></li></ol>';
        // Set the current, active page, and reveal it if it's hidden
        let current_page = document.location.href.toString().split("#")[0];
        if (current_page.endsWith("/")) {
            current_page += "index.html";
        }
        var links = Array.prototype.slice.call(this.querySelectorAll("a"));
        var l = links.length;
        for (var i = 0; i < l; ++i) {
            var link = links[i];
            var href = link.getAttribute("href");
            if (href && !href.startsWith("#") && !/^(?:[a-z+]+:)?\/\//.test(href)) {
                link.href = path_to_root + href;
            }
            // The "index" page is supposed to alias the first chapter in the book.
            if (link.href === current_page || (i === 0 && path_to_root === "" && current_page.endsWith("/index.html"))) {
                link.classList.add("active");
                var parent = link.parentElement;
                if (parent && parent.classList.contains("chapter-item")) {
                    parent.classList.add("expanded");
                }
                while (parent) {
                    if (parent.tagName === "LI" && parent.previousElementSibling) {
                        if (parent.previousElementSibling.classList.contains("chapter-item")) {
                            parent.previousElementSibling.classList.add("expanded");
                        }
                    }
                    parent = parent.parentElement;
                }
            }
        }
        // Track and set sidebar scroll position
        this.addEventListener('click', function(e) {
            if (e.target.tagName === 'A') {
                sessionStorage.setItem('sidebar-scroll', this.scrollTop);
            }
        }, { passive: true });
        var sidebarScrollTop = sessionStorage.getItem('sidebar-scroll');
        sessionStorage.removeItem('sidebar-scroll');
        if (sidebarScrollTop) {
            // preserve sidebar scroll position when navigating via links within sidebar
            this.scrollTop = sidebarScrollTop;
        } else {
            // scroll sidebar to current active section when navigating via "next/previous chapter" buttons
            var activeSection = document.querySelector('#sidebar .active');
            if (activeSection) {
                activeSection.scrollIntoView({ block: 'center' });
            }
        }
        // Toggle buttons
        var sidebarAnchorToggles = document.querySelectorAll('#sidebar a.toggle');
        function toggleSection(ev) {
            ev.currentTarget.parentElement.classList.toggle('expanded');
        }
        Array.from(sidebarAnchorToggles).forEach(function (el) {
            el.addEventListener('click', toggleSection);
        });
    }
}
window.customElements.define("mdbook-sidebar-scrollbox", MDBookSidebarScrollbox);
