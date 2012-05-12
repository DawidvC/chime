####Chime

Chime is a statically-compiled language that attempts to provide all of the productivity and dynamic features of a scripting language.

- Hand-written parser and lexer
- Statically-compiled with an LLVM backend
- Class-based with support for Traits
- Automatic memory management
- Very minimal host-system dependencies
- Dynamic runtime
- Support for closures
- A built-in build automation system
- Easy-to-learn syntax

You can read more about the concepts behind Chime and find documentation at the [chimelang.org](http://chimelang.org/ "Chime-Lang").

####Chime Code Style

The chime compiler is written in C++.  This was my first C++ project, and because of that, the code style is inconsistent.  As I learned and experimented, my style progressed.  In many places, newer style hasn't yet been applied to older sources.  I thought it could be useful to include some notes, just in case anyone is crazy enough to look through these sources.

There are some places in the compiler where memory management is horrendous.  I also only really began to understand and use the shared_ptr class later on.  I've come to depend on it for most memory management needs, but I haven't yet adopted it everywhere.

I've found C++ accessors to be particularly vexing.  I'm a very strong believer in encapsulation, so accessors are something I use heavily.  The style I've used has changed at least three times during the course of development.  My current style looks like this:

```C++
std::string name() const;
void        setName(const std::string& newName);
```

####Contributing

First, make sure you actually want to get involved with a project like this.  The philosophy behind Chime and its core library is weird.  Plus, there are lots of other great languages out there with huge and awesome communities.

If you really do want to work on this, I'd be thrilled to look at pull requests.

####License

Chime is nearly BSD-licensed.  The one exception is that I'd like to have control over distributions.  I also consider "distributions" to involve an unknown audience.  So, if you want to send a binary to a friend of yours, do it.  But, if you want to include modified chime binaries in some OS distribution, just shoot me a note and I'll say ok.

Human Version:

Do as you'd like with Chime, but if you'd like to distribute a modified version, please ask first.

Lawyer Version:

Redistribution and use in source and binary forms without modification are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
- Redistributions of modified source and binaries are forbidden without the expressed written consent of the current Chime maintainer, Matthew Massicotte.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
