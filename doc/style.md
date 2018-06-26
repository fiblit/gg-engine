# Turncoat C++ Style Guide (for beyond .clang-format)

##Philosophy

Important *themes* of my style:
0. Important information is presented on the *left*.
0. Whenever reasonable, prefer vertically compacted information as well.
0. Reading is more important than maintaining.
0. Write code for a future you with dementia, dyslexia, amnesia, and malice.
0. Reduce cognitive load.
0. Many indents smells like verbose abstractions or complexity. Refactor?
0. Encourage good idioms.
0. Don't be stupid, so ignore the guidelines as needed.

Anything remotely important enough to align should be on the left of my screen
where my eyeballs are reading vertically. Thus, important things go to the
left, and clutter goes to the right.

When my eyes are scanning they are reading vertically, not jumping sideways.
Within code, unlike text, horizontal jumps tend to be far more drastic, so I
prefer information to be compact and vertically oriented. I prefer this
because it lets me read more code as I'm scanning over it. This eases the 
need for page jumps which require cognitive load in recalling the purpose of
off-screen regions.

I generally agree with the *themes* of these statements from Linus Torvalds:
"If you need more than 3 levels of indentation, you're screwed anyway, and
should fix your program." which thus inspired my *'self-reflection'* theme; and
"You know you're brilliant but maybe you'd like to understand what you did 2
weeks from now.", which inspired my *'future you'* theme.

My style (as seen in my .clang-format) generally favors LLVM because of the
compactness and common lefthand alignment. Most exceptions are given to
removing items specific to the LLVM project or those which encourage having
important information on the right of the screen, contrary to my first theme.

---

##Testing

You should unit test with Catch2. Just because I'm not good at it doesn't mean
you should be too! Admittedly, I will set this up better sooner than later.


##Safety/Types

Try to use smart pointers and ownership semantics.
Generally use `unique_ptr` (rarely with weak `*` references to them)
Occasionally use `shared_ptr` with `weak_ptr` references to them.

---

##Naming

In general: `snake_case`. If hidden: `_under_snake_case`. If constant, global,
or important: `CAPS_CASE`. If it refers to an abstraction: `CamelCase`.

- Types: `CamelCase`
- Namespaces: `nospace //tend to be short`
- Functions:
    - Methods: `snake_case`
    - Private Methods: `_under_snake_case`
    - Namespaced: `snake_case`
    - Globals: `SNAKE_CASE`
    - File Static: `_under_snake_case`
    - Getter (rare): `Type var_name()`
    - Setter (rare): `void var_name(Type v)`
- Variables: 
    - Members: `_under_snake_case`
    - Locals: `snake_case`
    - Parameters: `snake_case_`
    - Globals: `CAPITAL_CASE`
    - Iters: `i j k`
    - Vectors: `(w) x y z, u v w`
    - Color: `r g b a, h s v`
    - Temps: `t s`
    - Randnum: `r`
    - File: `f`
    To clarify word boundaries, if it was originally hyphenated, use one word.
    e.g. Configuration-space -> C-Space -> _cspace
    If it's an acronym, treat like one word.
    Hyper Text Markup Language -> HTML -> _html
    Probabilistic Roadmap -> PRM -> Prm

##Whitespace
use
    (4 space indentation)
not
	(1 tab indentation)

##Bracing 

K&R -
    if (a) {
        return b;
    } else {
        return c;
    }

except functions -
    bool is_collide() {
    }

##Line continuation

extra indent continuations (halve?)
    if (a && abcdefghijklmnopqrstuvwxyz
            && x && z) {
        return "still got it!";
    }
    
    //declare
    void f(int a, int b, int c, int d,
        int f);

    //define
    void f(int a, int b, int c, int d,
            int f) {
        hey;
    }

##Header order

    //local
    #include "styleguide.h"
    //module
    #include "otherthing.h"
    #include "lower/thing.h"
    //project
    #include "../module/thing.h"
    //3rd pary
    #include <thirdparty/shenanigans>
    #include <glm/glm.hpp>
    #include <GLFW/glfw3.h>
    //system
    #include <memory>
    #include <vector>
    //c system
    #include <cstdlib>
    //OS - probably shouldn't need
    //#include <windows.h>
    
    //Forward declare.
    class Forward;
    class Soldiers;
    class March;

