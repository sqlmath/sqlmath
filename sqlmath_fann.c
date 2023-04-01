//!! #define FANN_EXTERNAL
//!! #define FANN_API

/*jslint-disable*/
/*
shRawLibFetch
{
    "fetchList": [
        {
            "comment": true,
            "url": "https://github.com/kaizhu256/fann/blob/v2021.2.21/LICENSE.md"
        },
        {
            "url": "https://github.com/kaizhu256/fann/blob/v2021.2.21/src/include/config.h"
        },
        {
            "url": "https://github.com/kaizhu256/fann/blob/v2021.2.21/src/include/doublefann.h"
        },
        {
            "url": "https://github.com/kaizhu256/fann/blob/v2021.2.21/src/include/fann_error.h"
        },
        {
            "url": "https://github.com/kaizhu256/fann/blob/v2021.2.21/src/include/fann_activation.h"
        },
        {
            "url": "https://github.com/kaizhu256/fann/blob/v2021.2.21/src/include/fann_data.h"
        },
        {
            "url": "https://github.com/kaizhu256/fann/blob/v2021.2.21/src/include/fann_internal.h"
        },
        {
            "url": "https://github.com/kaizhu256/fann/blob/v2021.2.21/src/include/fann_train.h"
        },
        {
            "url": "https://github.com/kaizhu256/fann/blob/v2021.2.21/src/include/fann_cascade.h"
        },
        {
            "url": "https://github.com/kaizhu256/fann/blob/v2021.2.21/src/include/fann_io.h"
        },
        {
            "url": "https://github.com/kaizhu256/fann/blob/v2021.2.21/src/include/fann.h"
        },
        {
            "url": "https://github.com/kaizhu256/fann/blob/v2021.2.21/src/include/parallel_fann.h"
        },
        {
            "url": "https://github.com/kaizhu256/fann/blob/v2021.2.21/src/fann.c"
        },
        {
            "url": "https://github.com/kaizhu256/fann/blob/v2021.2.21/src/fann_io.c"
        },
        {
            "url": "https://github.com/kaizhu256/fann/blob/v2021.2.21/src/fann_train.c"
        },
        {
            "url": "https://github.com/kaizhu256/fann/blob/v2021.2.21/src/fann_train_data.c"
        },
        {
            "url": "https://github.com/kaizhu256/fann/blob/v2021.2.21/src/fann_error.c"
        },
        {
            "url": "https://github.com/kaizhu256/fann/blob/v2021.2.21/src/fann_cascade.c"
        },
        {
            "url": "https://github.com/kaizhu256/fann/blob/v2021.2.21/src/parallel_fann.c"
        }
    ],
    "replaceList": [
        {
            "aa": "^#include \"(?:config|\\w*?fann\\w*?)\\.h\"",
            "bb": "// $&",
            "flags": "gm",
            "substr": ""
        }
    ]
}
*/


/*
repo https://github.com/kaizhu256/fann/tree/v2021.2.21
committed 2021-02-21T20:56:46Z
*/


/*
file https://github.com/kaizhu256/fann/blob/v2021.2.21/LICENSE.md
*/
/*
GNU LESSER GENERAL PUBLIC LICENSE
		       Version 2.1, February 1999

 Copyright (C) 1991, 1999 Free Software Foundation, Inc.
     59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 Everyone is permitted to copy and distribute verbatim copies
 of this license document, but changing it is not allowed.

[This is the first released version of the Lesser GPL.  It also counts
 as the successor of the GNU Library Public License, version 2, hence
 the version number 2.1.]

			    Preamble

  The licenses for most software are designed to take away your
freedom to share and change it.  By contrast, the GNU General Public
Licenses are intended to guarantee your freedom to share and change
free software--to make sure the software is free for all its users.

  This license, the Lesser General Public License, applies to some
specially designated software packages--typically libraries--of the
Free Software Foundation and other authors who decide to use it.  You
can use it too, but we suggest you first think carefully about whether
this license or the ordinary General Public License is the better
strategy to use in any particular case, based on the explanations below.

  When we speak of free software, we are referring to freedom of use,
not price.  Our General Public Licenses are designed to make sure that
you have the freedom to distribute copies of free software (and charge
for this service if you wish); that you receive source code or can get
it if you want it; that you can change the software and use pieces of
it in new free programs; and that you are informed that you can do
these things.

  To protect your rights, we need to make restrictions that forbid
distributors to deny you these rights or to ask you to surrender these
rights.  These restrictions translate to certain responsibilities for
you if you distribute copies of the library or if you modify it.

  For example, if you distribute copies of the library, whether gratis
or for a fee, you must give the recipients all the rights that we gave
you.  You must make sure that they, too, receive or can get the source
code.  If you link other code with the library, you must provide
complete object files to the recipients, so that they can relink them
with the library after making changes to the library and recompiling
it.  And you must show them these terms so they know their rights.

  We protect your rights with a two-step method: (1) we copyright the
library, and (2) we offer you this license, which gives you legal
permission to copy, distribute and/or modify the library.

  To protect each distributor, we want to make it very clear that
there is no warranty for the free library.  Also, if the library is
modified by someone else and passed on, the recipients should know
that what they have is not the original version, so that the original
author's reputation will not be affected by problems that might be
introduced by others.

  Finally, software patents pose a constant threat to the existence of
any free program.  We wish to make sure that a company cannot
effectively restrict the users of a free program by obtaining a
restrictive license from a patent holder.  Therefore, we insist that
any patent license obtained for a version of the library must be
consistent with the full freedom of use specified in this license.

  Most GNU software, including some libraries, is covered by the
ordinary GNU General Public License.  This license, the GNU Lesser
General Public License, applies to certain designated libraries, and
is quite different from the ordinary General Public License.  We use
this license for certain libraries in order to permit linking those
libraries into non-free programs.

  When a program is linked with a library, whether statically or using
a shared library, the combination of the two is legally speaking a
combined work, a derivative of the original library.  The ordinary
General Public License therefore permits such linking only if the
entire combination fits its criteria of freedom.  The Lesser General
Public License permits more lax criteria for linking other code with
the library.

  We call this license the "Lesser" General Public License because it
does Less to protect the user's freedom than the ordinary General
Public License.  It also provides other free software developers Less
of an advantage over competing non-free programs.  These disadvantages
are the reason we use the ordinary General Public License for many
libraries.  However, the Lesser license provides advantages in certain
special circumstances.

  For example, on rare occasions, there may be a special need to
encourage the widest possible use of a certain library, so that it becomes
a de-facto standard.  To achieve this, non-free programs must be
allowed to use the library.  A more frequent case is that a free
library does the same job as widely used non-free libraries.  In this
case, there is little to gain by limiting the free library to free
software only, so we use the Lesser General Public License.

  In other cases, permission to use a particular library in non-free
programs enables a greater number of people to use a large body of
free software.  For example, permission to use the GNU C Library in
non-free programs enables many more people to use the whole GNU
operating system, as well as its variant, the GNU/Linux operating
system.

  Although the Lesser General Public License is Less protective of the
users' freedom, it does ensure that the user of a program that is
linked with the Library has the freedom and the wherewithal to run
that program using a modified version of the Library.

  The precise terms and conditions for copying, distribution and
modification follow.  Pay close attention to the difference between a
"work based on the library" and a "work that uses the library".  The
former contains code derived from the library, whereas the latter must
be combined with the library in order to run.

		  GNU LESSER GENERAL PUBLIC LICENSE
   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION

  0. This License Agreement applies to any software library or other
program which contains a notice placed by the copyright holder or
other authorized party saying it may be distributed under the terms of
this Lesser General Public License (also called "this License").
Each licensee is addressed as "you".

  A "library" means a collection of software functions and/or data
prepared so as to be conveniently linked with application programs
(which use some of those functions and data) to form executables.

  The "Library", below, refers to any such software library or work
which has been distributed under these terms.  A "work based on the
Library" means either the Library or any derivative work under
copyright law: that is to say, a work containing the Library or a
portion of it, either verbatim or with modifications and/or translated
straightforwardly into another language.  (Hereinafter, translation is
included without limitation in the term "modification".)

  "Source code" for a work means the preferred form of the work for
making modifications to it.  For a library, complete source code means
all the source code for all modules it contains, plus any associated
interface definition files, plus the scripts used to control compilation
and installation of the library.

  Activities other than copying, distribution and modification are not
covered by this License; they are outside its scope.  The act of
running a program using the Library is not restricted, and output from
such a program is covered only if its contents constitute a work based
on the Library (independent of the use of the Library in a tool for
writing it).  Whether that is true depends on what the Library does
and what the program that uses the Library does.

  1. You may copy and distribute verbatim copies of the Library's
complete source code as you receive it, in any medium, provided that
you conspicuously and appropriately publish on each copy an
appropriate copyright notice and disclaimer of warranty; keep intact
all the notices that refer to this License and to the absence of any
warranty; and distribute a copy of this License along with the
Library.

  You may charge a fee for the physical act of transferring a copy,
and you may at your option offer warranty protection in exchange for a
fee.

  2. You may modify your copy or copies of the Library or any portion
of it, thus forming a work based on the Library, and copy and
distribute such modifications or work under the terms of Section 1
above, provided that you also meet all of these conditions:

    a) The modified work must itself be a software library.

    b) You must cause the files modified to carry prominent notices
    stating that you changed the files and the date of any change.

    c) You must cause the whole of the work to be licensed at no
    charge to all third parties under the terms of this License.

    d) If a facility in the modified Library refers to a function or a
    table of data to be supplied by an application program that uses
    the facility, other than as an argument passed when the facility
    is invoked, then you must make a good faith effort to ensure that,
    in the event an application does not supply such function or
    table, the facility still operates, and performs whatever part of
    its purpose remains meaningful.

    (For example, a function in a library to compute square roots has
    a purpose that is entirely well-defined independent of the
    application.  Therefore, Subsection 2d requires that any
    application-supplied function or table used by this function must
    be optional: if the application does not supply it, the square
    root function must still compute square roots.)

These requirements apply to the modified work as a whole.  If
identifiable sections of that work are not derived from the Library,
and can be reasonably considered independent and separate works in
themselves, then this License, and its terms, do not apply to those
sections when you distribute them as separate works.  But when you
distribute the same sections as part of a whole which is a work based
on the Library, the distribution of the whole must be on the terms of
this License, whose permissions for other licensees extend to the
entire whole, and thus to each and every part regardless of who wrote
it.

Thus, it is not the intent of this section to claim rights or contest
your rights to work written entirely by you; rather, the intent is to
exercise the right to control the distribution of derivative or
collective works based on the Library.

In addition, mere aggregation of another work not based on the Library
with the Library (or with a work based on the Library) on a volume of
a storage or distribution medium does not bring the other work under
the scope of this License.

  3. You may opt to apply the terms of the ordinary GNU General Public
License instead of this License to a given copy of the Library.  To do
this, you must alter all the notices that refer to this License, so
that they refer to the ordinary GNU General Public License, version 2,
instead of to this License.  (If a newer version than version 2 of the
ordinary GNU General Public License has appeared, then you can specify
that version instead if you wish.)  Do not make any other change in
these notices.

  Once this change is made in a given copy, it is irreversible for
that copy, so the ordinary GNU General Public License applies to all
subsequent copies and derivative works made from that copy.

  This option is useful when you wish to copy part of the code of
the Library into a program that is not a library.

  4. You may copy and distribute the Library (or a portion or
derivative of it, under Section 2) in object code or executable form
under the terms of Sections 1 and 2 above provided that you accompany
it with the complete corresponding machine-readable source code, which
must be distributed under the terms of Sections 1 and 2 above on a
medium customarily used for software interchange.

  If distribution of object code is made by offering access to copy
from a designated place, then offering equivalent access to copy the
source code from the same place satisfies the requirement to
distribute the source code, even though third parties are not
compelled to copy the source along with the object code.

  5. A program that contains no derivative of any portion of the
Library, but is designed to work with the Library by being compiled or
linked with it, is called a "work that uses the Library".  Such a
work, in isolation, is not a derivative work of the Library, and
therefore falls outside the scope of this License.

  However, linking a "work that uses the Library" with the Library
creates an executable that is a derivative of the Library (because it
contains portions of the Library), rather than a "work that uses the
library".  The executable is therefore covered by this License.
Section 6 states terms for distribution of such executables.

  When a "work that uses the Library" uses material from a header file
that is part of the Library, the object code for the work may be a
derivative work of the Library even though the source code is not.
Whether this is true is especially significant if the work can be
linked without the Library, or if the work is itself a library.  The
threshold for this to be true is not precisely defined by law.

  If such an object file uses only numerical parameters, data
structure layouts and accessors, and small macros and small inline
functions (ten lines or less in length), then the use of the object
file is unrestricted, regardless of whether it is legally a derivative
work.  (Executables containing this object code plus portions of the
Library will still fall under Section 6.)

  Otherwise, if the work is a derivative of the Library, you may
distribute the object code for the work under the terms of Section 6.
Any executables containing that work also fall under Section 6,
whether or not they are linked directly with the Library itself.

  6. As an exception to the Sections above, you may also combine or
link a "work that uses the Library" with the Library to produce a
work containing portions of the Library, and distribute that work
under terms of your choice, provided that the terms permit
modification of the work for the customer's own use and reverse
engineering for debugging such modifications.

  You must give prominent notice with each copy of the work that the
Library is used in it and that the Library and its use are covered by
this License.  You must supply a copy of this License.  If the work
during execution displays copyright notices, you must include the
copyright notice for the Library among them, as well as a reference
directing the user to the copy of this License.  Also, you must do one
of these things:

    a) Accompany the work with the complete corresponding
    machine-readable source code for the Library including whatever
    changes were used in the work (which must be distributed under
    Sections 1 and 2 above); and, if the work is an executable linked
    with the Library, with the complete machine-readable "work that
    uses the Library", as object code and/or source code, so that the
    user can modify the Library and then relink to produce a modified
    executable containing the modified Library.  (It is understood
    that the user who changes the contents of definitions files in the
    Library will not necessarily be able to recompile the application
    to use the modified definitions.)

    b) Use a suitable shared library mechanism for linking with the
    Library.  A suitable mechanism is one that (1) uses at run time a
    copy of the library already present on the user's computer system,
    rather than copying library functions into the executable, and (2)
    will operate properly with a modified version of the library, if
    the user installs one, as long as the modified version is
    interface-compatible with the version that the work was made with.

    c) Accompany the work with a written offer, valid for at
    least three years, to give the same user the materials
    specified in Subsection 6a, above, for a charge no more
    than the cost of performing this distribution.

    d) If distribution of the work is made by offering access to copy
    from a designated place, offer equivalent access to copy the above
    specified materials from the same place.

    e) Verify that the user has already received a copy of these
    materials or that you have already sent this user a copy.

  For an executable, the required form of the "work that uses the
Library" must include any data and utility programs needed for
reproducing the executable from it.  However, as a special exception,
the materials to be distributed need not include anything that is
normally distributed (in either source or binary form) with the major
components (compiler, kernel, and so on) of the operating system on
which the executable runs, unless that component itself accompanies
the executable.

  It may happen that this requirement contradicts the license
restrictions of other proprietary libraries that do not normally
accompany the operating system.  Such a contradiction means you cannot
use both them and the Library together in an executable that you
distribute.

  7. You may place library facilities that are a work based on the
Library side-by-side in a single library together with other library
facilities not covered by this License, and distribute such a combined
library, provided that the separate distribution of the work based on
the Library and of the other library facilities is otherwise
permitted, and provided that you do these two things:

    a) Accompany the combined library with a copy of the same work
    based on the Library, uncombined with any other library
    facilities.  This must be distributed under the terms of the
    Sections above.

    b) Give prominent notice with the combined library of the fact
    that part of it is a work based on the Library, and explaining
    where to find the accompanying uncombined form of the same work.

  8. You may not copy, modify, sublicense, link with, or distribute
the Library except as expressly provided under this License.  Any
attempt otherwise to copy, modify, sublicense, link with, or
distribute the Library is void, and will automatically terminate your
rights under this License.  However, parties who have received copies,
or rights, from you under this License will not have their licenses
terminated so long as such parties remain in full compliance.

  9. You are not required to accept this License, since you have not
signed it.  However, nothing else grants you permission to modify or
distribute the Library or its derivative works.  These actions are
prohibited by law if you do not accept this License.  Therefore, by
modifying or distributing the Library (or any work based on the
Library), you indicate your acceptance of this License to do so, and
all its terms and conditions for copying, distributing or modifying
the Library or works based on it.

  10. Each time you redistribute the Library (or any work based on the
Library), the recipient automatically receives a license from the
original licensor to copy, distribute, link with or modify the Library
subject to these terms and conditions.  You may not impose any further
restrictions on the recipients' exercise of the rights granted herein.
You are not responsible for enforcing compliance by third parties with
this License.

  11. If, as a consequence of a court judgment or allegation of patent
infringement or for any other reason (not limited to patent issues),
conditions are imposed on you (whether by court order, agreement or
otherwise) that contradict the conditions of this License, they do not
excuse you from the conditions of this License.  If you cannot
distribute so as to satisfy simultaneously your obligations under this
License and any other pertinent obligations, then as a consequence you
may not distribute the Library at all.  For example, if a patent
license would not permit royalty-free redistribution of the Library by
all those who receive copies directly or indirectly through you, then
the only way you could satisfy both it and this License would be to
refrain entirely from distribution of the Library.

If any portion of this section is held invalid or unenforceable under any
particular circumstance, the balance of the section is intended to apply,
and the section as a whole is intended to apply in other circumstances.

It is not the purpose of this section to induce you to infringe any
patents or other property right claims or to contest validity of any
such claims; this section has the sole purpose of protecting the
integrity of the free software distribution system which is
implemented by public license practices.  Many people have made
generous contributions to the wide range of software distributed
through that system in reliance on consistent application of that
system; it is up to the author/donor to decide if he or she is willing
to distribute software through any other system and a licensee cannot
impose that choice.

This section is intended to make thoroughly clear what is believed to
be a consequence of the rest of this License.

  12. If the distribution and/or use of the Library is restricted in
certain countries either by patents or by copyrighted interfaces, the
original copyright holder who places the Library under this License may add
an explicit geographical distribution limitation excluding those countries,
so that distribution is permitted only in or among countries not thus
excluded.  In such case, this License incorporates the limitation as if
written in the body of this License.

  13. The Free Software Foundation may publish revised and/or new
versions of the Lesser General Public License from time to time.
Such new versions will be similar in spirit to the present version,
but may differ in detail to address new problems or concerns.

Each version is given a distinguishing version number.  If the Library
specifies a version number of this License which applies to it and
"any later version", you have the option of following the terms and
conditions either of that version or of any later version published by
the Free Software Foundation.  If the Library does not specify a
license version number, you may choose any version ever published by
the Free Software Foundation.

  14. If you wish to incorporate parts of the Library into other free
programs whose distribution conditions are incompatible with these,
write to the author to ask for permission.  For software which is
copyrighted by the Free Software Foundation, write to the Free
Software Foundation; we sometimes make exceptions for this.  Our
decision will be guided by the two goals of preserving the free status
of all derivatives of our free software and of promoting the sharing
and reuse of software generally.

			    NO WARRANTY

  15. BECAUSE THE LIBRARY IS LICENSED FREE OF CHARGE, THERE IS NO
WARRANTY FOR THE LIBRARY, TO THE EXTENT PERMITTED BY APPLICABLE LAW.
EXCEPT WHEN OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR
OTHER PARTIES PROVIDE THE LIBRARY "AS IS" WITHOUT WARRANTY OF ANY
KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE.  THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE
LIBRARY IS WITH YOU.  SHOULD THE LIBRARY PROVE DEFECTIVE, YOU ASSUME
THE COST OF ALL NECESSARY SERVICING, REPAIR OR CORRECTION.

  16. IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN
WRITING WILL ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MAY MODIFY
AND/OR REDISTRIBUTE THE LIBRARY AS PERMITTED ABOVE, BE LIABLE TO YOU
FOR DAMAGES, INCLUDING ANY GENERAL, SPECIAL, INCIDENTAL OR
CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OR INABILITY TO USE THE
LIBRARY (INCLUDING BUT NOT LIMITED TO LOSS OF DATA OR DATA BEING
RENDERED INACCURATE OR LOSSES SUSTAINED BY YOU OR THIRD PARTIES OR A
FAILURE OF THE LIBRARY TO OPERATE WITH ANY OTHER SOFTWARE), EVEN IF
SUCH HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
DAMAGES.

		     END OF TERMS AND CONDITIONS

           How to Apply These Terms to Your New Libraries

  If you develop a new library, and you want it to be of the greatest
possible use to the public, we recommend making it free software that
everyone can redistribute and change.  You can do so by permitting
redistribution under these terms (or, alternatively, under the terms of the
ordinary General Public License).

  To apply these terms, attach the following notices to the library.  It is
safest to attach them to the start of each source file to most effectively
convey the exclusion of warranty; and each file should have at least the
"copyright" line and a pointer to where the full notice is found.

    <one line to give the library's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

Also add information on how to contact you by electronic and paper mail.

You should also get your employer (if you work as a programmer) or your
school, if any, to sign a "copyright disclaimer" for the library, if
necessary.  Here is a sample; alter the names:

  Yoyodyne, Inc., hereby disclaims all copyright interest in the
  library `Frob' (a library for tweaking knobs) written by James Random Hacker.

  <signature of Ty Coon>, 1 April 1990
  Ty Coon, President of Vice

That's all there is to it!
*/


/*
file https://github.com/kaizhu256/fann/blob/v2021.2.21/src/include/config.h
*/
/* Name of package */
/* #undef PACKAGE */

/* Version number of package */
#define VERSION "2.2.0"

/* Define for the x86_64 CPU famyly */
/* #undef X86_64 */


/*
file https://github.com/kaizhu256/fann/blob/v2021.2.21/src/include/doublefann.h
*/
/*
Fast Artificial Neural Network Library (fann)
Copyright (C) 2003-2016 Steffen Nissen (steffen.fann@gmail.com)

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef __doublefann_h__
#define __doublefann_h__

typedef double fann_type;

#undef DOUBLEFANN
#define DOUBLEFANN
#define FANNPRINTF "%.20e"
#define FANNSCANF "%le"

#define FANN_INCLUDE
// #include "fann.h"

#endif


/*
file https://github.com/kaizhu256/fann/blob/v2021.2.21/src/include/fann_error.h
*/
/*
Fast Artificial Neural Network Library (fann)
Copyright (C) 2003-2016 Steffen Nissen (steffen.fann@gmail.com)

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef __fann_error_h__
#define __fann_error_h__

#include <stdio.h>

#define FANN_ERRSTR_MAX 128
struct fann_error;

/* Section: FANN Error Handling

   Errors from the fann library are usually reported on stderr.
   It is however possible to redirect these error messages to a file,
   or completely ignore them by the <fann_set_error_log> function.

   It is also possible to inspect the last error message by using the
   <fann_get_errno> and <fann_get_errstr> functions.
 */

/* Enum: fann_errno_enum
	Used to define error events on <struct fann> and <struct fann_train_data>.

	See also:
		<fann_get_errno>, <fann_reset_errno>, <fann_get_errstr>

	FANN_E_NO_ERROR - No error
	FANN_E_CANT_OPEN_CONFIG_R - Unable to open configuration file for reading
	FANN_E_CANT_OPEN_CONFIG_W - Unable to open configuration file for writing
	FANN_E_WRONG_CONFIG_VERSION - Wrong version of configuration file
	FANN_E_CANT_READ_CONFIG - Error reading info from configuration file
	FANN_E_CANT_READ_NEURON - Error reading neuron info from configuration file
	FANN_E_CANT_READ_CONNECTIONS - Error reading connections from configuration file
	FANN_E_WRONG_NUM_CONNECTIONS - Number of connections not equal to the number expected
	FANN_E_CANT_OPEN_TD_W - Unable to open train data file for writing
	FANN_E_CANT_OPEN_TD_R - Unable to open train data file for reading
	FANN_E_CANT_READ_TD - Error reading training data from file
	FANN_E_CANT_ALLOCATE_MEM - Unable to allocate memory
	FANN_E_CANT_TRAIN_ACTIVATION - Unable to train with the selected activation function
	FANN_E_CANT_USE_ACTIVATION - Unable to use the selected activation function
	FANN_E_TRAIN_DATA_MISMATCH - Irreconcilable differences between two <struct fann_train_data> structures
	FANN_E_CANT_USE_TRAIN_ALG - Unable to use the selected training algorithm
	FANN_E_TRAIN_DATA_SUBSET - Trying to take subset which is not within the training set
	FANN_E_INDEX_OUT_OF_BOUND - Index is out of bound
	FANN_E_SCALE_NOT_PRESENT - Scaling parameters not present
    FANN_E_INPUT_NO_MATCH - The number of input neurons in the ann and data don't match
    FANN_E_OUTPUT_NO_MATCH - The number of output neurons in the ann and data don't match
	FANN_E_WRONG_PARAMETERS_FOR_CREATE - The parameters for create_standard are wrong, either too few parameters provided or a negative/very high value provided
*/
enum fann_errno_enum
{
	FANN_E_NO_ERROR = 0,
	FANN_E_CANT_OPEN_CONFIG_R,
	FANN_E_CANT_OPEN_CONFIG_W,
	FANN_E_WRONG_CONFIG_VERSION,
	FANN_E_CANT_READ_CONFIG,
	FANN_E_CANT_READ_NEURON,
	FANN_E_CANT_READ_CONNECTIONS,
	FANN_E_WRONG_NUM_CONNECTIONS,
	FANN_E_CANT_OPEN_TD_W,
	FANN_E_CANT_OPEN_TD_R,
	FANN_E_CANT_READ_TD,
	FANN_E_CANT_ALLOCATE_MEM,
	FANN_E_CANT_TRAIN_ACTIVATION,
	FANN_E_CANT_USE_ACTIVATION,
	FANN_E_TRAIN_DATA_MISMATCH,
	FANN_E_CANT_USE_TRAIN_ALG,
	FANN_E_TRAIN_DATA_SUBSET,
	FANN_E_INDEX_OUT_OF_BOUND,
	FANN_E_SCALE_NOT_PRESENT,
	FANN_E_INPUT_NO_MATCH,
	FANN_E_OUTPUT_NO_MATCH,
	FANN_E_WRONG_PARAMETERS_FOR_CREATE
};

/* Group: Error Handling */

/* Function: fann_set_error_log

   Change where errors are logged to. Both <struct fann> and <struct fann_data> can be
   casted to <struct fann_error>, so this function can be used to set either of these.

   If log_file is NULL, no errors will be printed.

   If errdat is NULL, the default log will be set. The default log is the log used when creating
   <struct fann> and <struct fann_data>. This default log will also be the default for all new structs
   that are created.

   The default behavior is to log them to stderr.

   See also:
    <struct fann_error>

   This function appears in FANN >= 1.1.0.
 */
FANN_EXTERNAL void FANN_API fann_set_error_log(struct fann_error *errdat, FILE * log_file);


/* Function: fann_get_errno

   Returns the last error number.

   See also:
    <fann_errno_enum>, <fann_reset_errno>

   This function appears in FANN >= 1.1.0.
 */
FANN_EXTERNAL enum fann_errno_enum FANN_API fann_get_errno(struct fann_error *errdat);


/* Function: fann_reset_errno

   Resets the last error number.

   This function appears in FANN >= 1.1.0.
 */
FANN_EXTERNAL void FANN_API fann_reset_errno(struct fann_error *errdat);


/* Function: fann_reset_errstr

   Resets the last error string.

   This function appears in FANN >= 1.1.0.
 */
FANN_EXTERNAL void FANN_API fann_reset_errstr(struct fann_error *errdat);


/* Function: fann_get_errstr

   Returns the last errstr.

   This function calls <fann_reset_errno> and <fann_reset_errstr>

   This function appears in FANN >= 1.1.0.
 */
FANN_EXTERNAL char *FANN_API fann_get_errstr(struct fann_error *errdat);


/* Function: fann_print_error

   Prints the last error to stderr.

   This function appears in FANN >= 1.1.0.
 */
FANN_EXTERNAL void FANN_API fann_print_error(struct fann_error *errdat);

FANN_EXTERNAL extern FILE * fann_default_error_log;

#endif


/*
file https://github.com/kaizhu256/fann/blob/v2021.2.21/src/include/fann_activation.h
*/
/*
Fast Artificial Neural Network Library (fann)
Copyright (C) 2003-2016 Steffen Nissen (steffen.fann@gmail.com)

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef __fann_activation_h__
#define __fann_activation_h__
/* internal include file, not to be included directly
*/

/* Implementation of the activation functions
*/

/* stepwise linear functions used for some of the activation functions */

/* defines used for the stepwise linear functions */

/* Uses the single precision version of math.h functions if
__doublefann_h__ is not defined
*/
#ifndef __doublefann_h__
#define FANN_EXP(x) expf(x)
#define FANN_SIN(x) sinf(x)
#define FANN_COS(x) cosf(x)
#else
#define FANN_EXP(x) exp(x)
#define FANN_SIN(x) sin(x)
#define FANN_COS(x) cos(x)
#endif

#define fann_linear_func(v1, r1, v2, r2, sum) (((((r2)-(r1)) * ((sum)-(v1)))/((v2)-(v1))) + (r1))
#define fann_stepwise(v1, v2, v3, v4, v5, v6, r1, r2, r3, r4, r5, r6, min, max, sum) (sum < v5 ? (sum < v3 ? (sum < v2 ? (sum < v1 ? min : fann_linear_func(v1, r1, v2, r2, sum)) : fann_linear_func(v2, r2, v3, r3, sum)) : (sum < v4 ? fann_linear_func(v3, r3, v4, r4, sum) : fann_linear_func(v4, r4, v5, r5, sum))) : (sum < v6 ? fann_linear_func(v5, r5, v6, r6, sum) : max))

/* FANN_LINEAR */
/* #define fann_linear(steepness, sum) fann_mult(steepness, sum) */
#define fann_linear_derive(steepness, value) (steepness)

/* FANN_SIGMOID */
/* #define fann_sigmoid(steepness, sum) (1.0f/(1.0f + exp(-2.0f * steepness * sum))) */
#define fann_sigmoid_real(sum) (1.0f/(1.0f + FANN_EXP(-2.0f * sum)))
#define fann_sigmoid_derive(steepness, value) (2.0f * steepness * value * (1.0f - value))

/* FANN_SIGMOID_SYMMETRIC */
/* #define fann_sigmoid_symmetric(steepness, sum) (2.0f/(1.0f + exp(-2.0f * steepness * sum)) - 1.0f) */
#define fann_sigmoid_symmetric_real(sum) (2.0f/(1.0f + FANN_EXP(-2.0f * sum)) - 1.0f)
#define fann_sigmoid_symmetric_derive(steepness, value) steepness * (1.0f - (value*value))

/* FANN_GAUSSIAN */
/* #define fann_gaussian(steepness, sum) (exp(-sum * steepness * sum * steepness)) */
#define fann_gaussian_real(sum) (FANN_EXP(-sum * sum))
#define fann_gaussian_derive(steepness, value, sum) (-2.0f * sum * value * steepness * steepness)

/* FANN_GAUSSIAN_SYMMETRIC */
/* #define fann_gaussian_symmetric(steepness, sum) ((exp(-sum * steepness * sum * steepness)*2.0)-1.0) */
#define fann_gaussian_symmetric_real(sum) ((FANN_EXP(-sum * sum)*2.0f)-1.0f)
#define fann_gaussian_symmetric_derive(steepness, value, sum) (-2.0f * sum * (value+1.0f) * steepness * steepness)

/* FANN_ELLIOT */
/* #define fann_elliot(steepness, sum) (((sum * steepness) / 2.0f) / (1.0f + fann_abs(sum * steepness)) + 0.5f) */
#define fann_elliot_real(sum) (((sum) / 2.0f) / (1.0f + fann_abs(sum)) + 0.5f)
#define fann_elliot_derive(steepness, value, sum) (steepness * 1.0f / (2.0f * (1.0f + fann_abs(sum)) * (1.0f + fann_abs(sum))))

/* FANN_ELLIOT_SYMMETRIC */
/* #define fann_elliot_symmetric(steepness, sum) ((sum * steepness) / (1.0f + fann_abs(sum * steepness)))*/
#define fann_elliot_symmetric_real(sum) ((sum) / (1.0f + fann_abs(sum)))
#define fann_elliot_symmetric_derive(steepness, value, sum) (steepness * 1.0f / ((1.0f + fann_abs(sum)) * (1.0f + fann_abs(sum))))

/* FANN_SIN_SYMMETRIC */
#define fann_sin_symmetric_real(sum) (FANN_SIN(sum))
#define fann_sin_symmetric_derive(steepness, sum) (steepness*cos(steepness*sum))

/* FANN_COS_SYMMETRIC */
#define fann_cos_symmetric_real(sum) (FANN_COS(sum))
#define fann_cos_symmetric_derive(steepness, sum) (steepness*-sin(steepness*sum))

/* FANN_SIN */
#define fann_sin_real(sum) (FANN_SIN(sum)/2.0f+0.5f)
#define fann_sin_derive(steepness, sum) (steepness*cos(steepness*sum)/2.0f)

/* FANN_COS */
#define fann_cos_real(sum) (FANN_COS(sum)/2.0f+0.5f)
#define fann_cos_derive(steepness, sum) (steepness*-sin(steepness*sum)/2.0f)

#define fann_activation_switch(activation_function, value, result) \
switch(activation_function) \
{ \
	case FANN_LINEAR: \
		result = (fann_type)value; \
        break; \
	case FANN_LINEAR_PIECE: \
		result = (fann_type)((value < 0) ? 0 : (value > 1) ? 1 : value); \
        break; \
	case FANN_LINEAR_PIECE_SYMMETRIC: \
		result = (fann_type)((value < -1) ? -1 : (value > 1) ? 1 : value); \
        break; \
	case FANN_SIGMOID: \
		result = (fann_type)fann_sigmoid_real(value); \
        break; \
	case FANN_SIGMOID_SYMMETRIC: \
		result = (fann_type)fann_sigmoid_symmetric_real(value); \
        break; \
	case FANN_SIGMOID_SYMMETRIC_STEPWISE: \
		result = (fann_type)fann_stepwise(((fann_type)-2.64665293693542480469e+00), ((fann_type)-1.47221934795379638672e+00), ((fann_type)-5.49306154251098632812e-01), ((fann_type)5.49306154251098632812e-01), ((fann_type)1.47221934795379638672e+00), ((fann_type)2.64665293693542480469e+00), ((fann_type)-9.90000009536743164062e-01), ((fann_type)-8.99999976158142089844e-01), ((fann_type)-5.00000000000000000000e-01), ((fann_type)5.00000000000000000000e-01), ((fann_type)8.99999976158142089844e-01), ((fann_type)9.90000009536743164062e-01), -1, 1, value); \
        break; \
	case FANN_SIGMOID_STEPWISE: \
		result = (fann_type)fann_stepwise(((fann_type)-2.64665246009826660156e+00), ((fann_type)-1.47221946716308593750e+00), ((fann_type)-5.49306154251098632812e-01), ((fann_type)5.49306154251098632812e-01), ((fann_type)1.47221934795379638672e+00), ((fann_type)2.64665293693542480469e+00), ((fann_type) 4.99999988824129104614e-03), ((fann_type) 5.00000007450580596924e-02), ((fann_type) 2.50000000000000000000e-01), ((fann_type)7.50000000000000000000e-01), ((fann_type)9.49999988079071044922e-01), ((fann_type)9.95000004768371582031e-01), 0, 1, value); \
        break; \
	case FANN_THRESHOLD: \
		result = (fann_type)((value < 0) ? 0 : 1); \
        break; \
	case FANN_THRESHOLD_SYMMETRIC: \
		result = (fann_type)((value < 0) ? -1 : 1); \
        break; \
	case FANN_GAUSSIAN: \
		result = (fann_type)fann_gaussian_real(value); \
        break; \
	case FANN_GAUSSIAN_SYMMETRIC: \
		result = (fann_type)fann_gaussian_symmetric_real(value); \
        break; \
	case FANN_ELLIOT: \
		result = (fann_type)fann_elliot_real(value); \
	    break; \
	case FANN_ELLIOT_SYMMETRIC: \
		result = (fann_type)fann_elliot_symmetric_real(value); \
        break; \
	case FANN_SIN_SYMMETRIC: \
		result = (fann_type)fann_sin_symmetric_real(value); \
        break; \
	case FANN_COS_SYMMETRIC: \
		result = (fann_type)fann_cos_symmetric_real(value); \
        break; \
	case FANN_SIN: \
		result = (fann_type)fann_sin_real(value); \
        break; \
	case FANN_COS: \
		result = (fann_type)fann_cos_real(value); \
        break; \
	case FANN_GAUSSIAN_STEPWISE: \
        result = 0; \
        break; \
}

#endif


/*
file https://github.com/kaizhu256/fann/blob/v2021.2.21/src/include/fann_data.h
*/
/*
Fast Artificial Neural Network Library (fann)
Copyright (C) 2003-2016 Steffen Nissen (steffen.fann@gmail.com)

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef __fann_data_h__
#define __fann_data_h__

#include <stdio.h>

/* Section: FANN Datatypes

   The two main datatypes used in the fann library are <struct fann>,
   which represents an artificial neural network, and <struct fann_train_data>,
   which represents training data.
 */


/* Type: fann_type
   fann_type is the type used for the weights, inputs and outputs of the neural network.

	fann_type is defined as a:
	float - if you include fann.h or floatfann.h
	double - if you include doublefann.h
	int - if you include fixedfann.h (please be aware that fixed point usage is
			only to be used during execution, and not during training).
*/

/* Enum: fann_train_enum
	The Training algorithms used when training on <struct fann_train_data> with functions like
	<fann_train_on_data> or <fann_train_on_file>. The incremental training alters the weights
	after each time it is presented an input pattern, while batch only alters the weights once after
	it has been presented to all the patterns.

	FANN_TRAIN_INCREMENTAL -  Standard backpropagation algorithm, where the weights are
		updated after each training pattern. This means that the weights are updated many
		times during a single epoch. For this reason some problems will train very fast with
		this algorithm, while other more advanced problems will not train very well.
	FANN_TRAIN_BATCH -  Standard backpropagation algorithm, where the weights are updated after
		calculating the mean square error for the whole training set. This means that the weights
		are only updated once during an epoch. For this reason some problems will train slower with
		this algorithm. But since the mean square error is calculated more correctly than in
		incremental training, some problems will reach better solutions with this algorithm.
	FANN_TRAIN_RPROP - A more advanced batch training algorithm which achieves good results
		for many problems. The RPROP training algorithm is adaptive, and does therefore not
		use the learning_rate. Some other parameters can however be set to change the way the
		RPROP algorithm works, but it is only recommended for users with insight in how the RPROP
		training algorithm works. The RPROP training algorithm is described by
		[Riedmiller and Braun, 1993], but the actual learning algorithm used here is the
		iRPROP- training algorithm which is described by [Igel and Husken, 2000] which
		is a variant of the standard RPROP training algorithm.
	FANN_TRAIN_QUICKPROP - A more advanced batch training algorithm which achieves good results
		for many problems. The quickprop training algorithm uses the learning_rate parameter
		along with other more advanced parameters, but it is only recommended to change these
		advanced parameters, for users with insight in how the quickprop training algorithm works.
		The quickprop training algorithm is described by [Fahlman, 1988].
	FANN_TRAIN_SARPROP - THE SARPROP ALGORITHM: A SIMULATED ANNEALING ENHANCEMENT TO RESILIENT BACK PROPAGATION
    http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.47.8197&rep=rep1&type=pdf

	See also:
		<fann_set_training_algorithm>, <fann_get_training_algorithm>
*/
enum fann_train_enum
{
	FANN_TRAIN_INCREMENTAL = 0,
	FANN_TRAIN_BATCH,
	FANN_TRAIN_RPROP,
	FANN_TRAIN_QUICKPROP,
	FANN_TRAIN_SARPROP
};

/* Constant: FANN_TRAIN_NAMES

   Constant array consisting of the names for the training algorithms, so that the name of an
   training function can be received by:
   (code)
   char *name = FANN_TRAIN_NAMES[train_function];
   (end)

   See Also:
      <fann_train_enum>
*/
static char const *const FANN_TRAIN_NAMES[] = {
	"FANN_TRAIN_INCREMENTAL",
	"FANN_TRAIN_BATCH",
	"FANN_TRAIN_RPROP",
	"FANN_TRAIN_QUICKPROP",
	"FANN_TRAIN_SARPROP"
};

/* Enums: fann_activationfunc_enum

	The activation functions used for the neurons during training. The activation functions
	can either be defined for a group of neurons by <fann_set_activation_function_hidden> and
	<fann_set_activation_function_output> or it can be defined for a single neuron by <fann_set_activation_function>.

	The steepness of an activation function is defined in the same way by
	<fann_set_activation_steepness_hidden>, <fann_set_activation_steepness_output> and <fann_set_activation_steepness>.

   The functions are described with functions where:
   * x is the input to the activation function,
   * y is the output,
   * s is the steepness and
   * d is the derivation.

   FANN_LINEAR - Linear activation function.
     * span: -inf < y < inf
	 * y = x*s, d = 1*s
	 * Can NOT be used in fixed point.

   FANN_THRESHOLD - Threshold activation function.
	 * x < 0 -> y = 0, x >= 0 -> y = 1
	 * Can NOT be used during training.

   FANN_THRESHOLD_SYMMETRIC - Threshold activation function.
	 * x < 0 -> y = -1, x >= 0 -> y = 1
	 * Can NOT be used during training.

   FANN_SIGMOID - Sigmoid activation function.
	 * One of the most used activation functions.
	 * span: 0 < y < 1
	 * y = 1/(1 + exp(-2*s*x))
	 * d = 2*s*y*(1 - y)

   FANN_SIGMOID_STEPWISE - Stepwise linear approximation to sigmoid.
	 * Faster than sigmoid but a bit less precise.

   FANN_SIGMOID_SYMMETRIC - Symmetric sigmoid activation function, aka. tanh.
	 * One of the most used activation functions.
	 * span: -1 < y < 1
	 * y = tanh(s*x) = 2/(1 + exp(-2*s*x)) - 1
	 * d = s*(1-(y*y))

   FANN_SIGMOID_SYMMETRIC_STEPWISE - Stepwise linear approximation to symmetric sigmoid.
	 * Faster than symmetric sigmoid but a bit less precise.

   FANN_GAUSSIAN - Gaussian activation function.
	 * 0 when x = -inf, 1 when x = 0 and 0 when x = inf
	 * span: 0 < y < 1
	 * y = exp(-x*s*x*s)
	 * d = -2*x*s*y*s

   FANN_GAUSSIAN_SYMMETRIC - Symmetric gaussian activation function.
	 * -1 when x = -inf, 1 when x = 0 and 0 when x = inf
	 * span: -1 < y < 1
	 * y = exp(-x*s*x*s)*2-1
	 * d = -2*x*s*(y+1)*s

   FANN_ELLIOT - Fast (sigmoid like) activation function defined by David Elliott
	 * span: 0 < y < 1
	 * y = ((x*s) / 2) / (1 + |x*s|) + 0.5
	 * d = s*1/(2*(1+|x*s|)*(1+|x*s|))

   FANN_ELLIOT_SYMMETRIC - Fast (symmetric sigmoid like) activation function defined by David Elliott
	 * span: -1 < y < 1
	 * y = (x*s) / (1 + |x*s|)
	 * d = s*1/((1+|x*s|)*(1+|x*s|))

	FANN_LINEAR_PIECE - Bounded linear activation function.
	 * span: 0 <= y <= 1
	 * y = x*s, d = 1*s

	FANN_LINEAR_PIECE_SYMMETRIC - Bounded linear activation function.
	 * span: -1 <= y <= 1
	 * y = x*s, d = 1*s

	FANN_SIN_SYMMETRIC - Periodical sinus activation function.
	 * span: -1 <= y <= 1
	 * y = sin(x*s)
	 * d = s*cos(x*s)

	FANN_COS_SYMMETRIC - Periodical cosinus activation function.
	 * span: -1 <= y <= 1
	 * y = cos(x*s)
	 * d = s*-sin(x*s)

	FANN_SIN - Periodical sinus activation function.
	 * span: 0 <= y <= 1
	 * y = sin(x*s)/2+0.5
	 * d = s*cos(x*s)/2

	FANN_COS - Periodical cosinus activation function.
	 * span: 0 <= y <= 1
	 * y = cos(x*s)/2+0.5
	 * d = s*-sin(x*s)/2

	See also:
   	<fann_set_activation_function_layer>, <fann_set_activation_function_hidden>,
   	<fann_set_activation_function_output>, <fann_set_activation_steepness>,
    <fann_set_activation_function>
*/
enum fann_activationfunc_enum
{
	FANN_LINEAR = 0,
	FANN_THRESHOLD,
	FANN_THRESHOLD_SYMMETRIC,
	FANN_SIGMOID,
	FANN_SIGMOID_STEPWISE,
	FANN_SIGMOID_SYMMETRIC,
	FANN_SIGMOID_SYMMETRIC_STEPWISE,
	FANN_GAUSSIAN,
	FANN_GAUSSIAN_SYMMETRIC,
	/* Stepwise linear approximation to gaussian.
	 * Faster than gaussian but a bit less precise.
	 * NOT implemented yet.
	 */
	FANN_GAUSSIAN_STEPWISE,
	FANN_ELLIOT,
	FANN_ELLIOT_SYMMETRIC,
	FANN_LINEAR_PIECE,
	FANN_LINEAR_PIECE_SYMMETRIC,
	FANN_SIN_SYMMETRIC,
	FANN_COS_SYMMETRIC,
	FANN_SIN,
	FANN_COS
};

/* Constant: FANN_ACTIVATIONFUNC_NAMES

   Constant array consisting of the names for the activation function, so that the name of an
   activation function can be received by:
   (code)
   char *name = FANN_ACTIVATIONFUNC_NAMES[activation_function];
   (end)

   See Also:
      <fann_activationfunc_enum>
*/
static char const *const FANN_ACTIVATIONFUNC_NAMES[] = {
	"FANN_LINEAR",
	"FANN_THRESHOLD",
	"FANN_THRESHOLD_SYMMETRIC",
	"FANN_SIGMOID",
	"FANN_SIGMOID_STEPWISE",
	"FANN_SIGMOID_SYMMETRIC",
	"FANN_SIGMOID_SYMMETRIC_STEPWISE",
	"FANN_GAUSSIAN",
	"FANN_GAUSSIAN_SYMMETRIC",
	"FANN_GAUSSIAN_STEPWISE",
	"FANN_ELLIOT",
	"FANN_ELLIOT_SYMMETRIC",
	"FANN_LINEAR_PIECE",
	"FANN_LINEAR_PIECE_SYMMETRIC",
	"FANN_SIN_SYMMETRIC",
	"FANN_COS_SYMMETRIC",
	"FANN_SIN",
	"FANN_COS"
};

/* Enum: fann_errorfunc_enum
	Error function used during training.

	FANN_ERRORFUNC_LINEAR - Standard linear error function.
	FANN_ERRORFUNC_TANH - Tanh error function, usually better
		but can require a lower learning rate. This error function aggressively targets outputs that
		differ much from the desired, while not targeting outputs that only differ a little that much.
		This activation function is not recommended for cascade training and incremental training.

	See also:
		<fann_set_train_error_function>, <fann_get_train_error_function>
*/
enum fann_errorfunc_enum
{
	FANN_ERRORFUNC_LINEAR = 0,
	FANN_ERRORFUNC_TANH
};

/* Constant: FANN_ERRORFUNC_NAMES

   Constant array consisting of the names for the training error functions, so that the name of an
   error function can be received by:
   (code)
   char *name = FANN_ERRORFUNC_NAMES[error_function];
   (end)

   See Also:
      <fann_errorfunc_enum>
*/
static char const *const FANN_ERRORFUNC_NAMES[] = {
	"FANN_ERRORFUNC_LINEAR",
	"FANN_ERRORFUNC_TANH"
};

/* Enum: fann_stopfunc_enum
	Stop criteria used during training.

	FANN_STOPFUNC_MSE - Stop criterion is Mean Square Error (MSE) value.
	FANN_STOPFUNC_BIT - Stop criterion is number of bits that fail. The number of bits; means the
		number of output neurons which differ more than the bit fail limit
		(see <fann_get_bit_fail_limit>, <fann_set_bit_fail_limit>).
		The bits are counted in all of the training data, so this number can be higher than
		the number of training data.

	See also:
		<fann_set_train_stop_function>, <fann_get_train_stop_function>
*/
enum fann_stopfunc_enum
{
	FANN_STOPFUNC_MSE = 0,
	FANN_STOPFUNC_BIT
};

/* Constant: FANN_STOPFUNC_NAMES

   Constant array consisting of the names for the training stop functions, so that the name of a
   stop function can be received by:
   (code)
   char *name = FANN_STOPFUNC_NAMES[stop_function];
   (end)

   See Also:
      <fann_stopfunc_enum>
*/
static char const *const FANN_STOPFUNC_NAMES[] = {
	"FANN_STOPFUNC_MSE",
	"FANN_STOPFUNC_BIT"
};

/* Enum: fann_network_type_enum

    Definition of network types used by <fann_get_network_type>

    FANN_NETTYPE_LAYER - Each layer only has connections to the next layer
    FANN_NETTYPE_SHORTCUT - Each layer has connections to all following layers

   See Also:
      <fann_get_network_type>

   This enumeration appears in FANN >= 2.1.0
*/
enum fann_nettype_enum
{
    FANN_NETTYPE_LAYER = 0, /* Each layer only has connections to the next layer */
    FANN_NETTYPE_SHORTCUT /* Each layer has connections to all following layers */
};

/* Constant: FANN_NETWORK_TYPE_NAMES

   Constant array consisting of the names for the network types, so that the name of an
   network type can be received by:
   (code)
   char *network_type_name = FANN_NETWORK_TYPE_NAMES[fann_get_network_type(ann)];
   (end)

   See Also:
      <fann_get_network_type>

   This constant appears in FANN >= 2.1.0
*/
static char const *const FANN_NETTYPE_NAMES[] = {
	"FANN_NETTYPE_LAYER",
	"FANN_NETTYPE_SHORTCUT"
};


/* forward declarations for use with the callback */
struct fann;
struct fann_train_data;
/* Type: fann_callback_type
   This callback function can be called during training when using <fann_train_on_data>,
   <fann_train_on_file> or <fann_cascadetrain_on_data>.

	>typedef int (FANN_API * fann_callback_type) (struct fann *ann, struct fann_train_data *train,
	>											  unsigned int max_epochs,
	>                                             unsigned int epochs_between_reports,
	>                                             float desired_error, unsigned int epochs);

	The callback can be set by using <fann_set_callback> and is very useful for doing custom
	things during training. It is recommended to use this function when implementing custom
	training procedures, or when visualizing the training in a GUI etc. The parameters which the
	callback function takes are the parameters given to <fann_train_on_data>, plus an epochs
	parameter which tells how many epochs the training has taken so far.

	The callback function should return an integer, if the callback function returns -1, the training
	will terminate.

	Example of a callback function:
		>int FANN_API test_callback(struct fann *ann, struct fann_train_data *train,
		>				            unsigned int max_epochs, unsigned int epochs_between_reports,
		>				            float desired_error, unsigned int epochs)
		>{
		>	printf("Epochs     %8d. MSE: %.5f. Desired-MSE: %.5f\n", epochs, fann_get_MSE(ann), desired_error);
		>	return 0;
		>}

	See also:
		<fann_set_callback>, <fann_train_on_data>
 */
FANN_EXTERNAL typedef int (FANN_API * fann_callback_type) (struct fann *ann, struct fann_train_data *train,
														   unsigned int max_epochs,
														   unsigned int epochs_between_reports,
														   float desired_error, unsigned int epochs);


/* ----- Data structures -----
 * No data within these structures should be altered directly by the user.
 */

struct fann_neuron
{
	/* Index to the first and last connection
	 * (actually the last is a past end index)
	 */
	unsigned int first_con;
	unsigned int last_con;
	/* The sum of the inputs multiplied with the weights */
	fann_type sum;
	/* The value of the activation function applied to the sum */
	fann_type value;
	/* The steepness of the activation function */
	fann_type activation_steepness;
	/* Used to choose which activation function to use */
	enum fann_activationfunc_enum activation_function;
#ifdef __GNUC__
} __attribute__ ((packed));
#else
};
#endif

/* A single layer in the neural network.
 */
struct fann_layer
{
	/* A pointer to the first neuron in the layer
	 * When allocated, all the neurons in all the layers are actually
	 * in one long array, this is because we want to easily clear all
	 * the neurons at once.
	 */
	struct fann_neuron *first_neuron;

	/* A pointer to the neuron past the last neuron in the layer */
	/* the number of neurons is last_neuron - first_neuron */
	struct fann_neuron *last_neuron;
};

/* Struct: struct fann_error

	Structure used to store error-related information, both
	<struct fann> and <struct fann_train_data> can be casted to this type.

	See also:
		<fann_set_error_log>, <fann_get_errno>
*/
struct fann_error
{
	enum fann_errno_enum errno_f;
	FILE *error_log;
	char *errstr;
};


/* 	Struct: struct fann
	The fast artificial neural network (fann) structure.

	Data within this structure should never be accessed directly, but only by using the
	*fann_get_...* and *fann_set_...* functions.

	The fann structure is created using one of the *fann_create_...* functions and each of
	the functions which operates on the structure takes *struct fann * ann* as the first parameter.

	See also:
		<fann_create_standard>, <fann_destroy>
 */
struct fann
{
	/* The type of error that last occured. */
	enum fann_errno_enum errno_f;

	/* Where to log error messages. */
	FILE *error_log;

	/* A string representation of the last error. */
	char *errstr;

	/* the learning rate of the network */
	float learning_rate;

	/* The learning momentum used for backpropagation algorithm. */
	float learning_momentum;

	/* the connection rate of the network
	 * between 0 and 1, 1 meaning fully connected
	 */
	float connection_rate;

	/* is 1 if shortcut connections are used in the ann otherwise 0
	 * Shortcut connections are connections that skip layers.
	 * A fully connected ann with shortcut connections are a ann where
	 * neurons have connections to all neurons in all later layers.
	 */
	enum fann_nettype_enum network_type;

	/* pointer to the first layer (input layer) in an array af all the layers,
	 * including the input and outputlayers
	 */
	struct fann_layer *first_layer;

	/* pointer to the layer past the last layer in an array af all the layers,
	 * including the input and outputlayers
	 */
	struct fann_layer *last_layer;

	/* Total number of neurons.
	 * very useful, because the actual neurons are allocated in one long array
	 */
	unsigned int total_neurons;

	/* Number of input neurons (not calculating bias) */
	unsigned int num_input;

	/* Number of output neurons (not calculating bias) */
	unsigned int num_output;

	/* The weight array */
	fann_type *weights;

	/* The connection array */
	struct fann_neuron **connections;

	/* Used to contain the errors used during training
	 * Is allocated during first training session,
	 * which means that if we do not train, it is never allocated.
	 */
	fann_type *train_errors;

	/* Training algorithm used when calling fann_train_on_..
	 */
	enum fann_train_enum training_algorithm;

#ifdef FIXEDFANN
	/* the decimal_point, used for shifting the fix point
	 * in fixed point integer operatons.
	 */
	unsigned int decimal_point;

	/* the multiplier, used for multiplying the fix point
	 * in fixed point integer operatons.
	 * Only used in special cases, since the decimal_point is much faster.
	 */
	unsigned int multiplier;

	/* When in choosen (or in fixed point), the sigmoid function is
	 * calculated as a stepwise linear function. In the
	 * activation_results array, the result is saved, and in the
	 * two values arrays, the values that gives the results are saved.
	 */
	fann_type sigmoid_results[6];
	fann_type sigmoid_values[6];
	fann_type sigmoid_symmetric_results[6];
	fann_type sigmoid_symmetric_values[6];
#endif

	/* Total number of connections.
	 * very useful, because the actual connections
	 * are allocated in one long array
	 */
	unsigned int total_connections;

	/* used to store outputs in */
	fann_type *output;

	/* the number of data used to calculate the mean square error.
	 */
	unsigned int num_MSE;

	/* the total error value.
	 * the real mean square error is MSE_value/num_MSE
	 */
	float MSE_value;

	/* The number of outputs which would fail (only valid for classification problems)
	 */
	unsigned int num_bit_fail;

	/* The maximum difference between the actual output and the expected output
	 * which is accepted when counting the bit fails.
	 * This difference is multiplied by two when dealing with symmetric activation functions,
	 * so that symmetric and not symmetric activation functions can use the same limit.
	 */
	fann_type bit_fail_limit;

	/* The error function used during training. (default FANN_ERRORFUNC_TANH)
	 */
	enum fann_errorfunc_enum train_error_function;

	/* The stop function used during training. (default FANN_STOPFUNC_MSE)
	*/
	enum fann_stopfunc_enum train_stop_function;

	/* The callback function used during training. (default NULL)
	*/
	fann_callback_type callback;

    /* A pointer to user defined data. (default NULL)
    */
    void *user_data;

    /* Variables for use with Cascade Correlation */

	/* The error must change by at least this
	 * fraction of its old value to count as a
	 * significant change.
	 */
	float cascade_output_change_fraction;

	/* No change in this number of epochs will cause
	 * stagnation.
	 */
	unsigned int cascade_output_stagnation_epochs;

	/* The error must change by at least this
	 * fraction of its old value to count as a
	 * significant change.
	 */
	float cascade_candidate_change_fraction;

	/* No change in this number of epochs will cause
	 * stagnation.
	 */
	unsigned int cascade_candidate_stagnation_epochs;

	/* The current best candidate, which will be installed.
	 */
	unsigned int cascade_best_candidate;

	/* The upper limit for a candidate score
	 */
	fann_type cascade_candidate_limit;

	/* Scale of copied candidate output weights
	 */
	fann_type cascade_weight_multiplier;

	/* Maximum epochs to train the output neurons during cascade training
	 */
	unsigned int cascade_max_out_epochs;

	/* Maximum epochs to train the candidate neurons during cascade training
	 */
	unsigned int cascade_max_cand_epochs;

	/* Minimum epochs to train the output neurons during cascade training
	 */
	unsigned int cascade_min_out_epochs;

	/* Minimum epochs to train the candidate neurons during cascade training
	 */
	unsigned int cascade_min_cand_epochs;

	/* An array consisting of the activation functions used when doing
	 * cascade training.
	 */
	enum fann_activationfunc_enum *cascade_activation_functions;

	/* The number of elements in the cascade_activation_functions array.
	*/
	unsigned int cascade_activation_functions_count;

	/* An array consisting of the steepnesses used during cascade training.
	*/
	fann_type *cascade_activation_steepnesses;

	/* The number of elements in the cascade_activation_steepnesses array.
	*/
	unsigned int cascade_activation_steepnesses_count;

	/* The number of candidates of each type that will be present.
	 * The actual number of candidates is then
	 * cascade_activation_functions_count *
	 * cascade_activation_steepnesses_count *
	 * cascade_num_candidate_groups
	*/
	unsigned int cascade_num_candidate_groups;

	/* An array consisting of the score of the individual candidates,
	 * which is used to decide which candidate is the best
	 */
	fann_type *cascade_candidate_scores;

	/* The number of allocated neurons during cascade correlation algorithms.
	 * This number might be higher than the actual number of neurons to avoid
	 * allocating new space too often.
	 */
	unsigned int total_neurons_allocated;

	/* The number of allocated connections during cascade correlation algorithms.
	 * This number might be higher than the actual number of neurons to avoid
	 * allocating new space too often.
	 */
	unsigned int total_connections_allocated;

	/* Variables for use with Quickprop training */

	/* Decay is used to make the weights not go so high */
	float quickprop_decay;

	/* Mu is a factor used to increase and decrease the stepsize */
	float quickprop_mu;

	/* Variables for use with with RPROP training */

	/* Tells how much the stepsize should increase during learning */
	float rprop_increase_factor;

	/* Tells how much the stepsize should decrease during learning */
	float rprop_decrease_factor;

	/* The minimum stepsize */
	float rprop_delta_min;

	/* The maximum stepsize */
	float rprop_delta_max;

	/* The initial stepsize */
	float rprop_delta_zero;

	/* Defines how much the weights are constrained to smaller values at the beginning */
	float sarprop_weight_decay_shift;

	/* Decides if the stepsize is too big with regard to the error */
	float sarprop_step_error_threshold_factor;

	/* Defines how much the stepsize is influenced by the error */
	float sarprop_step_error_shift;

	/* Defines how much the epoch influences weight decay and noise */
	float sarprop_temperature;

	/* Current training epoch */
	unsigned int sarprop_epoch;

	/* Used to contain the slope errors used during batch training
	 * Is allocated during first training session,
	 * which means that if we do not train, it is never allocated.
	 */
	fann_type *train_slopes;

	/* The previous step taken by the quickprop/rprop procedures.
	 * Not allocated if not used.
	 */
	fann_type *prev_steps;

	/* The slope values used by the quickprop/rprop procedures.
	 * Not allocated if not used.
	 */
	fann_type *prev_train_slopes;

	/* The last delta applied to a connection weight.
	 * This is used for the momentum term in the backpropagation algorithm.
	 * Not allocated if not used.
	 */
	fann_type *prev_weights_deltas;

#ifndef FIXEDFANN
	/* Arithmetic mean used to remove steady component in input data.  */
	float *scale_mean_in;

	/* Standart deviation used to normalize input data (mostly to [-1;1]). */
	float *scale_deviation_in;

	/* User-defined new minimum for input data.
	 * Resulting data values may be less than user-defined minimum.
	 */
	float *scale_new_min_in;

	/* Used to scale data to user-defined new maximum for input data.
	 * Resulting data values may be greater than user-defined maximum.
	 */
	float *scale_factor_in;

	/* Arithmetic mean used to remove steady component in output data.  */
	float *scale_mean_out;

	/* Standart deviation used to normalize output data (mostly to [-1;1]). */
	float *scale_deviation_out;

	/* User-defined new minimum for output data.
	 * Resulting data values may be less than user-defined minimum.
	 */
	float *scale_new_min_out;

	/* Used to scale data to user-defined new maximum for output data.
	 * Resulting data values may be greater than user-defined maximum.
	 */
	float *scale_factor_out;
#endif
};

/* Type: fann_connection

    Describes a connection between two neurons and its weight

    from_neuron - Unique number used to identify source neuron
    to_neuron - Unique number used to identify destination neuron
    weight - The numerical value of the weight

    See Also:
        <fann_get_connection_array>, <fann_set_weight_array>

   This structure appears in FANN >= 2.1.0
*/
struct fann_connection
{
    /* Unique number used to identify source neuron */
    unsigned int from_neuron;
    /* Unique number used to identify destination neuron */
    unsigned int to_neuron;
    /* The numerical value of the weight */
    fann_type weight;
};

#endif


/*
file https://github.com/kaizhu256/fann/blob/v2021.2.21/src/include/fann_internal.h
*/
/*
Fast Artificial Neural Network Library (fann)
Copyright (C) 2003-2016 Steffen Nissen (steffen.fann@gmail.com)

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef __fann_internal_h__
#define __fann_internal_h__
/* internal include file, not to be included directly
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
// #include "fann_data.h"

#define FANN_FIX_VERSION "FANN_FIX_2.0"
#define FANN_FLO_VERSION "FANN_FLO_2.1"

#ifdef FIXEDFANN
#define FANN_CONF_VERSION FANN_FIX_VERSION
#else
#define FANN_CONF_VERSION FANN_FLO_VERSION
#endif

#define FANN_GET(type, name) \
FANN_EXTERNAL type FANN_API fann_get_ ## name(struct fann *ann) \
{ \
	return ann->name; \
}

#define FANN_SET(type, name) \
FANN_EXTERNAL void FANN_API fann_set_ ## name(struct fann *ann, type value) \
{ \
	ann->name = value; \
}

#define FANN_GET_SET(type, name) \
FANN_GET(type, name) \
FANN_SET(type, name)


struct fann_train_data;

struct fann *fann_allocate_structure(unsigned int num_layers);
void fann_allocate_neurons(struct fann *ann);

void fann_allocate_connections(struct fann *ann);

int fann_save_internal(struct fann *ann, const char *configuration_file,
					   unsigned int save_as_fixed);
int fann_save_internal_fd(struct fann *ann, FILE * conf, const char *configuration_file,
						  unsigned int save_as_fixed);
int fann_save_train_internal(struct fann_train_data *data, const char *filename,
							  unsigned int save_as_fixed, unsigned int decimal_point);
int fann_save_train_internal_fd(struct fann_train_data *data, FILE * file, const char *filename,
								 unsigned int save_as_fixed, unsigned int decimal_point);

void fann_update_stepwise(struct fann *ann);
void fann_seed_rand();

void fann_error(struct fann_error *errdat, const enum fann_errno_enum errno_f, ...);
void fann_init_error_data(struct fann_error *errdat);

struct fann *fann_create_from_fd(FILE * conf, const char *configuration_file);
struct fann_train_data *fann_read_train_from_fd(FILE * file, const char *filename);

void fann_compute_MSE(struct fann *ann, fann_type * desired_output);
void fann_update_output_weights(struct fann *ann);
void fann_backpropagate_MSE(struct fann *ann);
void fann_update_weights(struct fann *ann);
void fann_update_slopes_batch(struct fann *ann, struct fann_layer *layer_begin,
							  struct fann_layer *layer_end);
void fann_update_weights_quickprop(struct fann *ann, unsigned int num_data,
								   unsigned int first_weight, unsigned int past_end);
void fann_update_weights_batch(struct fann *ann, unsigned int num_data, unsigned int first_weight,
							   unsigned int past_end);
void fann_update_weights_irpropm(struct fann *ann, unsigned int first_weight,
								 unsigned int past_end);
void fann_update_weights_sarprop(struct fann *ann, unsigned int epoch, unsigned int first_weight,
								unsigned int past_end);

void fann_clear_train_arrays(struct fann *ann);

fann_type fann_activation(struct fann * ann, unsigned int activation_function, fann_type steepness,
						  fann_type value);

fann_type fann_activation_derived(unsigned int activation_function,
								  fann_type steepness, fann_type value, fann_type sum);

int fann_desired_error_reached(struct fann *ann, float desired_error);

/* Some functions for cascade */
int fann_train_outputs(struct fann *ann, struct fann_train_data *data, float desired_error);

float fann_train_outputs_epoch(struct fann *ann, struct fann_train_data *data);

int fann_train_candidates(struct fann *ann, struct fann_train_data *data);

fann_type fann_train_candidates_epoch(struct fann *ann, struct fann_train_data *data);

void fann_install_candidate(struct fann *ann);
int fann_check_input_output_sizes(struct fann *ann, struct fann_train_data *data);

int fann_initialize_candidates(struct fann *ann);

void fann_set_shortcut_connections(struct fann *ann);

int fann_allocate_scale(struct fann *ann);

FANN_EXTERNAL void FANN_API fann_scale_data_to_range(fann_type ** data, unsigned int num_data, unsigned int num_elem,
					 fann_type old_min, fann_type old_max, fann_type new_min, fann_type new_max);

/* called fann_max, in order to not interferre with predefined versions of max */
#define fann_max(x, y) (((x) > (y)) ? (x) : (y))
#define fann_min(x, y) (((x) < (y)) ? (x) : (y))
#define fann_safe_free(x) {if(x) { free(x); x = NULL; }}
#define fann_clip(x, lo, hi) (((x) < (lo)) ? (lo) : (((x) > (hi)) ? (hi) : (x)))
#define fann_exp2(x) exp(0.69314718055994530942*(x))
/*#define fann_clip(x, lo, hi) (x)*/

#define fann_rand(min_value, max_value) (((float)(min_value))+(((float)(max_value)-((float)(min_value)))*rand()/(RAND_MAX+1.0f)))

#define fann_abs(value) (((value) > 0) ? (value) : -(value))

#ifdef FIXEDFANN

#define fann_mult(x,y) ((x*y) >> decimal_point)
#define fann_div(x,y) (((x) << decimal_point)/y)
#define fann_random_weight() (fann_type)(fann_rand(0,multiplier/10))
#define fann_random_bias_weight() (fann_type)(fann_rand((0-multiplier)/10,multiplier/10))

#else

#define fann_mult(x,y) (x*y)
#define fann_div(x,y) (x/y)
#define fann_random_weight() (fann_rand(-0.1f,0.1f))
#define fann_random_bias_weight() (fann_rand(-0.1f,0.1f))

#endif

#endif


/*
file https://github.com/kaizhu256/fann/blob/v2021.2.21/src/include/fann_train.h
*/
/*
Fast Artificial Neural Network Library (fann)
Copyright (C) 2003-2016 Steffen Nissen (steffen.fann@gmail.com)

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef __fann_train_h__
#define __fann_train_h__

/* Section: FANN Training

 	There are many different ways of training neural networks and the FANN library supports
 	a number of different approaches.

 	Two fundamentally different approaches are the most commonly used:

 		Fixed topology training - The size and topology of the ANN is determined in advance
 			and the training alters the weights in order to minimize the difference between
 			the desired output values and the actual output values. This kind of training is
 			supported by <fann_train_on_data>.

 		Evolving topology training - The training start out with an empty ANN, only consisting
 			of input and output neurons. Hidden neurons and connections are added during training,
 			in order to reach the same goal as for fixed topology training. This kind of training
 			is supported by <FANN Cascade Training>.
 */

/* Struct: struct fann_train_data
	Structure used to store data, for use with training.

	The data inside this structure should never be manipulated directly, but should use some
	of the supplied functions in <Training Data Manipulation>.

	The training data structure is very usefull for storing data during training and testing of a
	neural network.

	See also:
	<fann_read_train_from_file>, <fann_train_on_data>, <fann_destroy_train>
*/
struct fann_train_data
{
	enum fann_errno_enum errno_f;
	FILE *error_log;
	char *errstr;

	unsigned int num_data;
	unsigned int num_input;
	unsigned int num_output;
	fann_type **input;
	fann_type **output;
};

/* Section: FANN Training */

/* Group: Training */

#ifndef FIXEDFANN
/* Function: fann_train

   Train one iteration with a set of inputs, and a set of desired outputs.
   This training is always incremental training (see <fann_train_enum>), since
   only one pattern is presented.

   Parameters:
   	ann - The neural network structure
   	input - an array of inputs. This array must be exactly <fann_get_num_input> long.
   	desired_output - an array of desired outputs. This array must be exactly <fann_get_num_output> long.

   	See also:
   		<fann_train_on_data>, <fann_train_epoch>

   	This function appears in FANN >= 1.0.0.
 */
FANN_EXTERNAL void FANN_API fann_train(struct fann *ann, fann_type * input,
									   fann_type * desired_output);

#endif	/* NOT FIXEDFANN */

/* Function: fann_test
   Test with a set of inputs, and a set of desired outputs.
   This operation updates the mean square error, but does not
   change the network in any way.

   See also:
   		<fann_test_data>, <fann_train>

   This function appears in FANN >= 1.0.0.
*/
FANN_EXTERNAL fann_type * FANN_API fann_test(struct fann *ann, fann_type * input,
												 fann_type * desired_output);

/* Function: fann_get_MSE
   Reads the mean square error from the network.

   Reads the mean square error from the network. This value is calculated during
   training or testing, and can therefore sometimes be a bit off if the weights
   have been changed since the last calculation of the value.

   See also:
   	<fann_test_data>

	This function appears in FANN >= 1.1.0.
 */
FANN_EXTERNAL float FANN_API fann_get_MSE(struct fann *ann);

/* Function: fann_get_bit_fail

	The number of fail bits; means the number of output neurons which differ more
	than the bit fail limit (see <fann_get_bit_fail_limit>, <fann_set_bit_fail_limit>).
	The bits are counted in all of the training data, so this number can be higher than
	the number of training data.

	This value is reset by <fann_reset_MSE> and updated by all the same functions which also
	update the MSE value (e.g. <fann_test_data>, <fann_train_epoch>)

	See also:
		<fann_stopfunc_enum>, <fann_get_MSE>

	This function appears in FANN >= 2.0.0
*/
FANN_EXTERNAL unsigned int FANN_API fann_get_bit_fail(struct fann *ann);

/* Function: fann_reset_MSE
   Resets the mean square error from the network.

   This function also resets the number of bits that fail.

   See also:
   	<fann_get_MSE>, <fann_get_bit_fail_limit>

    This function appears in FANN >= 1.1.0
 */
FANN_EXTERNAL void FANN_API fann_reset_MSE(struct fann *ann);

/* Group: Training Data Training */

#ifndef FIXEDFANN

/* Function: fann_train_on_data

   Trains on an entire dataset, for a period of time.

   This training uses the training algorithm chosen by <fann_set_training_algorithm>,
   and the parameters set for these training algorithms.

   Parameters:
   		ann - The neural network
   		data - The data, which should be used during training
   		max_epochs - The maximum number of epochs the training should continue
   		epochs_between_reports - The number of epochs between printing a status report to stdout.
   			A value of zero means no reports should be printed.
   		desired_error - The desired <fann_get_MSE> or <fann_get_bit_fail>, depending on which stop function
   			is chosen by <fann_set_train_stop_function>.

	Instead of printing out reports every epochs_between_reports, a callback function can be called
	(see <fann_set_callback>).

	See also:
		<fann_train_on_file>, <fann_train_epoch>, <Parameters>

	This function appears in FANN >= 1.0.0.
*/
FANN_EXTERNAL void FANN_API fann_train_on_data(struct fann *ann, struct fann_train_data *data,
											   unsigned int max_epochs,
											   unsigned int epochs_between_reports,
											   float desired_error);

/* Function: fann_train_on_file

   Does the same as <fann_train_on_data>, but reads the training data directly from a file.

   See also:
   		<fann_train_on_data>

	This function appears in FANN >= 1.0.0.
*/
FANN_EXTERNAL void FANN_API fann_train_on_file(struct fann *ann, const char *filename,
											   unsigned int max_epochs,
											   unsigned int epochs_between_reports,
											   float desired_error);

/* Function: fann_train_epoch
   Train one epoch with a set of training data.

    Train one epoch with the training data stored in data. One epoch is where all of
    the training data is considered exactly once.

	This function returns the MSE error as it is calculated either before or during
	the actual training. This is not the actual MSE after the training epoch, but since
	calculating this will require to go through the entire training set once more, it is
	more than adequate to use this value during training.

	The training algorithm used by this function is chosen by the <fann_set_training_algorithm>
	function.

	See also:
		<fann_train_on_data>, <fann_test_data>

	This function appears in FANN >= 1.2.0.
 */
FANN_EXTERNAL float FANN_API fann_train_epoch(struct fann *ann, struct fann_train_data *data);
#endif	/* NOT FIXEDFANN */

/* Function: fann_test_data

   Test a set of training data and calculates the MSE for the training data.

   This function updates the MSE and the bit fail values.

   See also:
 	<fann_test>, <fann_get_MSE>, <fann_get_bit_fail>

	This function appears in FANN >= 1.2.0.
 */
FANN_EXTERNAL float FANN_API fann_test_data(struct fann *ann, struct fann_train_data *data);

/* Group: Training Data Manipulation */

/* Function: fann_read_train_from_file
   Reads a file that stores training data.

   The file must be formatted like:
   >num_train_data num_input num_output
   >inputdata separated by space
   >outputdata separated by space
   >
   >.
   >.
   >.
   >
   >inputdata separated by space
   >outputdata separated by space

   See also:
   	<fann_train_on_data>, <fann_destroy_train>, <fann_save_train>

    This function appears in FANN >= 1.0.0
*/
FANN_EXTERNAL struct fann_train_data *FANN_API fann_read_train_from_file(const char *filename);


/* Function: fann_create_train
   Creates an empty training data struct.

   See also:
     <fann_read_train_from_file>, <fann_train_on_data>, <fann_destroy_train>,
     <fann_save_train>, <fann_create_train_array>

    This function appears in FANN >= 2.2.0
*/
FANN_EXTERNAL struct fann_train_data * FANN_API fann_create_train(unsigned int num_data, unsigned int num_input, unsigned int num_output);

/* Function: fann_create_train_pointer_array
   Creates an training data struct and fills it with data from provided arrays of pointer.

   A copy of the data is made so there are no restrictions on the
   allocation of the input/output data and the caller is responsible
   for the deallocation of the data pointed to by input and output.

   See also:
     <fann_read_train_from_file>, <fann_train_on_data>, <fann_destroy_train>,
     <fann_save_train>, <fann_create_train>, <fann_create_train_array>

    This function appears in FANN >= 2.3.0
*/
FANN_EXTERNAL struct fann_train_data * FANN_API fann_create_train_pointer_array(unsigned int num_data, unsigned int num_input, fann_type **input, unsigned int num_output, fann_type **output);

/* Function: fann_create_train_array
   Creates an training data struct and fills it with data from provided arrays, where the arrays must have the dimensions:
   input[num_data*num_input]
   output[num_data*num_output]

   A copy of the data is made so there are no restrictions on the
   allocation of the input/output data and the caller is responsible
   for the deallocation of the data pointed to by input and output.

   See also:
     <fann_read_train_from_file>, <fann_train_on_data>, <fann_destroy_train>,
     <fann_save_train>, <fann_create_train>, <fann_create_train_pointer_array>

    This function appears in FANN >= 2.3.0
*/
FANN_EXTERNAL struct fann_train_data * FANN_API fann_create_train_array(unsigned int num_data, unsigned int num_input, fann_type *input, unsigned int num_output, fann_type *output);

/* Function: fann_create_train_from_callback
   Creates the training data struct from a user supplied function.
   As the training data are numerable (data 1, data 2...), the user must write
   a function that receives the number of the training data set (input,output)
   and returns the set.  fann_create_train_from_callback will call the user
   supplied function 'num_data' times, one input-output pair each time. Each
   time the user supplied function is called, the time of the call will be passed
   as the 'num' parameter and the user supplied function must write the input
   and output to the corresponding parameters.


   Parameters:
     num_data      - The number of training data
     num_input     - The number of inputs per training data
     num_output    - The number of ouputs per training data
     user_function - The user supplied function

   Parameters for the user function:
     num        - The number of the training data set
     num_input  - The number of inputs per training data
     num_output - The number of ouputs per training data
     input      - The set of inputs
     output     - The set of desired outputs

   See also:
     <fann_read_train_from_file>, <fann_train_on_data>, <fann_destroy_train>,
     <fann_save_train>

    This function appears in FANN >= 2.1.0
*/
FANN_EXTERNAL struct fann_train_data * FANN_API fann_create_train_from_callback(unsigned int num_data,
                                          unsigned int num_input,
                                          unsigned int num_output,
                                          void (FANN_API *user_function)( unsigned int,
                                                                 unsigned int,
                                                                 unsigned int,
                                                                 fann_type * ,
                                                                 fann_type * ));

/* Function: fann_destroy_train
   Destructs the training data and properly deallocates all of the associated data.
   Be sure to call this function when finished using the training data.

    This function appears in FANN >= 1.0.0
 */
FANN_EXTERNAL void FANN_API fann_destroy_train(struct fann_train_data *train_data);

/* Function: fann_get_train_input
   Gets the training input data at the given position

   See also:
     <fann_get_train_output>

   This function appears in FANN >= 2.3.0
 */
FANN_EXTERNAL fann_type * FANN_API fann_get_train_input(struct fann_train_data * data, unsigned int position);

/* Function: fann_get_train_output
   Gets the training output data at the given position

   See also:
     <fann_get_train_output>

   This function appears in FANN >= 2.3.0
 */
FANN_EXTERNAL fann_type * FANN_API fann_get_train_output(struct fann_train_data * data, unsigned int position);


/* Function: fann_shuffle_train_data

   Shuffles training data, randomizing the order.
   This is recommended for incremental training, while it has no influence during batch training.

   This function appears in FANN >= 1.1.0.
 */
FANN_EXTERNAL void FANN_API fann_shuffle_train_data(struct fann_train_data *train_data);

#ifndef FIXEDFANN

/* Function: fann_get_min_train_input

   Get the minimum value of all in the input data

   This function appears in FANN >= 2.3.0
*/
FANN_EXTERNAL fann_type FANN_API fann_get_min_train_input(struct fann_train_data *train_data);

/* Function: fann_get_max_train_input

   Get the maximum value of all in the input data

   This function appears in FANN >= 2.3.0
*/
FANN_EXTERNAL fann_type FANN_API fann_get_max_train_input(struct fann_train_data *train_data);

/* Function: fann_get_min_train_output

   Get the minimum value of all in the output data

   This function appears in FANN >= 2.3.0
*/
FANN_EXTERNAL fann_type FANN_API fann_get_min_train_output(struct fann_train_data *train_data);

/* Function: fann_get_max_train_output

   Get the maximum value of all in the output data

   This function appears in FANN >= 2.3.0
*/
FANN_EXTERNAL fann_type FANN_API fann_get_max_train_output(struct fann_train_data *train_data);


/* Function: fann_scale_train

   Scale input and output data based on previously calculated parameters.

   Parameters:
     ann      - ann for which trained parameters were calculated before
     data     - training data that needs to be scaled

   See also:
   	<fann_descale_train>, <fann_set_scaling_params>

    This function appears in FANN >= 2.1.0
*/
FANN_EXTERNAL void FANN_API fann_scale_train( struct fann *ann, struct fann_train_data *data );

/* Function: fann_descale_train

   Descale input and output data based on previously calculated parameters.

   Parameters:
     ann      - ann for which trained parameters were calculated before
     data     - training data that needs to be descaled

   See also:
   	<fann_scale_train>, <fann_set_scaling_params>

    This function appears in FANN >= 2.1.0
 */
FANN_EXTERNAL void FANN_API fann_descale_train( struct fann *ann, struct fann_train_data *data );

/* Function: fann_set_input_scaling_params

   Calculate input scaling parameters for future use based on training data.

   Parameters:
   	 ann           - ann for which parameters need to be calculated
   	 data          - training data that will be used to calculate scaling parameters
   	 new_input_min - desired lower bound in input data after scaling (not strictly followed)
   	 new_input_max - desired upper bound in input data after scaling (not strictly followed)

   See also:
   	 <fann_set_output_scaling_params>

    This function appears in FANN >= 2.1.0
 */
FANN_EXTERNAL int FANN_API fann_set_input_scaling_params(
	struct fann *ann,
	const struct fann_train_data *data,
	float new_input_min,
	float new_input_max);

/* Function: fann_set_output_scaling_params

   Calculate output scaling parameters for future use based on training data.

   Parameters:
   	 ann            - ann for which parameters need to be calculated
   	 data           - training data that will be used to calculate scaling parameters
   	 new_output_min - desired lower bound in output data after scaling (not strictly followed)
   	 new_output_max - desired upper bound in output data after scaling (not strictly followed)

   See also:
   	 <fann_set_input_scaling_params>

    This function appears in FANN >= 2.1.0
 */
FANN_EXTERNAL int FANN_API fann_set_output_scaling_params(
	struct fann *ann,
	const struct fann_train_data *data,
	float new_output_min,
	float new_output_max);

/* Function: fann_set_scaling_params

   Calculate input and output scaling parameters for future use based on training data.

   Parameters:
   	 ann            - ann for which parameters need to be calculated
   	 data           - training data that will be used to calculate scaling parameters
   	 new_input_min  - desired lower bound in input data after scaling (not strictly followed)
   	 new_input_max  - desired upper bound in input data after scaling (not strictly followed)
   	 new_output_min - desired lower bound in output data after scaling (not strictly followed)
   	 new_output_max - desired upper bound in output data after scaling (not strictly followed)

   See also:
   	 <fann_set_input_scaling_params>, <fann_set_output_scaling_params>

    This function appears in FANN >= 2.1.0
 */
FANN_EXTERNAL int FANN_API fann_set_scaling_params(
	struct fann *ann,
	const struct fann_train_data *data,
	float new_input_min,
	float new_input_max,
	float new_output_min,
	float new_output_max);

/* Function: fann_clear_scaling_params

   Clears scaling parameters.

   Parameters:
     ann - ann for which to clear scaling parameters

    This function appears in FANN >= 2.1.0
 */
FANN_EXTERNAL int FANN_API fann_clear_scaling_params(struct fann *ann);

/* Function: fann_scale_input

   Scale data in input vector before feeding it to ann based on previously calculated parameters.

   Parameters:
     ann          - for which scaling parameters were calculated
     input_vector - input vector that will be scaled

   See also:
     <fann_descale_input>, <fann_scale_output>

    This function appears in FANN >= 2.1.0
*/
FANN_EXTERNAL void FANN_API fann_scale_input( struct fann *ann, fann_type *input_vector );

/* Function: fann_scale_output

   Scale data in output vector before feeding it to ann based on previously calculated parameters.

   Parameters:
     ann           - for which scaling parameters were calculated
     output_vector - output vector that will be scaled

   See also:
     <fann_descale_output>, <fann_scale_input>

    This function appears in FANN >= 2.1.0
 */
FANN_EXTERNAL void FANN_API fann_scale_output( struct fann *ann, fann_type *output_vector );

/* Function: fann_descale_input

   Scale data in input vector after getting it from ann based on previously calculated parameters.

   Parameters:
     ann          - for which scaling parameters were calculated
     input_vector - input vector that will be descaled

   See also:
     <fann_scale_input>, <fann_descale_output>

    This function appears in FANN >= 2.1.0
 */
FANN_EXTERNAL void FANN_API fann_descale_input( struct fann *ann, fann_type *input_vector );

/* Function: fann_descale_output

   Scale data in output vector after getting it from ann based on previously calculated parameters.

   Parameters:
     ann           - for which scaling parameters were calculated
     output_vector - output vector that will be descaled

   See also:
     <fann_scale_output>, <fann_descale_input>

    This function appears in FANN >= 2.1.0
 */
FANN_EXTERNAL void FANN_API fann_descale_output( struct fann *ann, fann_type *output_vector );

#endif

/* Function: fann_scale_input_train_data

   Scales the inputs in the training data to the specified range.

   A simplified scaling method, which is mostly useful in examples where it's known that all the
   data will be in one range and it should be transformed to another range.

   It is not recommended to use this on subsets of data as the complete input range might not be
   available in that subset.

   For more powerful scaling, please consider <fann_scale_train>

   See also:
   	<fann_scale_output_train_data>, <fann_scale_train_data>, <fann_scala_input>

   This function appears in FANN >= 2.0.0.
 */
FANN_EXTERNAL void FANN_API fann_scale_input_train_data(struct fann_train_data *train_data,
														fann_type new_min, fann_type new_max);


/* Function: fann_scale_output_train_data

   Scales the outputs in the training data to the specified range.

   A simplified scaling method, which is mostly useful in examples where it's known that all the
   data will be in one range and it should be transformed to another range.

   It is not recommended to use this on subsets of data as the complete input range might not be
   available in that subset.

   For more powerful scaling, please consider <fann_scale_train>

   See also:
   	<fann_scale_input_train_data>, <fann_scale_train_data>

   This function appears in FANN >= 2.0.0.
 */
FANN_EXTERNAL void FANN_API fann_scale_output_train_data(struct fann_train_data *train_data,
														 fann_type new_min, fann_type new_max);


/* Function: fann_scale_train_data

   Scales the inputs and outputs in the training data to the specified range.

   A simplified scaling method, which is mostly useful in examples where it's known that all the
   data will be in one range and it should be transformed to another range.

   It is not recommended to use this on subsets of data as the complete input range might not be
   available in that subset.

   For more powerful scaling, please consider <fann_scale_train>

   See also:
   	<fann_scale_output_train_data>, <fann_scale_input_train_data>

   This function appears in FANN >= 2.0.0.
 */
FANN_EXTERNAL void FANN_API fann_scale_train_data(struct fann_train_data *train_data,
												  fann_type new_min, fann_type new_max);


/* Function: fann_merge_train_data

   Merges the data from *data1* and *data2* into a new <struct fann_train_data>.

   This function appears in FANN >= 1.1.0.
 */
FANN_EXTERNAL struct fann_train_data *FANN_API fann_merge_train_data(struct fann_train_data *data1,
																	 struct fann_train_data *data2);


/* Function: fann_duplicate_train_data

   Returns an exact copy of a <struct fann_train_data>.

   This function appears in FANN >= 1.1.0.
 */
FANN_EXTERNAL struct fann_train_data *FANN_API fann_duplicate_train_data(struct fann_train_data
																		 *data);

/* Function: fann_subset_train_data

   Returns an copy of a subset of the <struct fann_train_data>, starting at position *pos*
   and *length* elements forward.

   >fann_subset_train_data(train_data, 0, fann_length_train_data(train_data))

   Will do the same as <fann_duplicate_train_data>.

   See also:
   	<fann_length_train_data>

   This function appears in FANN >= 2.0.0.
 */
FANN_EXTERNAL struct fann_train_data *FANN_API fann_subset_train_data(struct fann_train_data
																		 *data, unsigned int pos,
																		 unsigned int length);

/* Function: fann_length_train_data

   Returns the number of training patterns in the <struct fann_train_data>.

   This function appears in FANN >= 2.0.0.
 */
FANN_EXTERNAL unsigned int FANN_API fann_length_train_data(struct fann_train_data *data);

/* Function: fann_num_input_train_data

   Returns the number of inputs in each of the training patterns in the <struct fann_train_data>.

   See also:
   	<fann_num_train_data>, <fann_num_output_train_data>

   This function appears in FANN >= 2.0.0.
 */
FANN_EXTERNAL unsigned int FANN_API fann_num_input_train_data(struct fann_train_data *data);

/* Function: fann_num_output_train_data

   Returns the number of outputs in each of the training patterns in the <struct fann_train_data>.

   See also:
   	<fann_num_train_data>, <fann_num_input_train_data>

   This function appears in FANN >= 2.0.0.
 */
FANN_EXTERNAL unsigned int FANN_API fann_num_output_train_data(struct fann_train_data *data);

/* Function: fann_save_train

   Save the training structure to a file, with the format as specified in <fann_read_train_from_file>

   Return:
   The function returns 0 on success and -1 on failure.

   See also:
   	<fann_read_train_from_file>, <fann_save_train_to_fixed>

   This function appears in FANN >= 1.0.0.
 */
FANN_EXTERNAL int FANN_API fann_save_train(struct fann_train_data *data, const char *filename);


/* Function: fann_save_train_to_fixed

   Saves the training structure to a fixed point data file.

   This function is very useful for testing the quality of a fixed point network.

   Return:
   The function returns 0 on success and -1 on failure.

   See also:
   	<fann_save_train>

   This function appears in FANN >= 1.0.0.
 */
FANN_EXTERNAL int FANN_API fann_save_train_to_fixed(struct fann_train_data *data, const char *filename,
													 unsigned int decimal_point);


/* Group: Parameters */

/* Function: fann_get_training_algorithm

   Return the training algorithm as described by <fann_train_enum>. This training algorithm
   is used by <fann_train_on_data> and associated functions.

   Note that this algorithm is also used during <fann_cascadetrain_on_data>, although only
   FANN_TRAIN_RPROP and FANN_TRAIN_QUICKPROP is allowed during cascade training.

   The default training algorithm is FANN_TRAIN_RPROP.

   See also:
    <fann_set_training_algorithm>, <fann_train_enum>

   This function appears in FANN >= 1.0.0.
 */
FANN_EXTERNAL enum fann_train_enum FANN_API fann_get_training_algorithm(struct fann *ann);


/* Function: fann_set_training_algorithm

   Set the training algorithm.

   More info available in <fann_get_training_algorithm>

   This function appears in FANN >= 1.0.0.
 */
FANN_EXTERNAL void FANN_API fann_set_training_algorithm(struct fann *ann,
														enum fann_train_enum training_algorithm);


/* Function: fann_get_learning_rate

   Return the learning rate.

   The learning rate is used to determine how aggressive training should be for some of the
   training algorithms (FANN_TRAIN_INCREMENTAL, FANN_TRAIN_BATCH, FANN_TRAIN_QUICKPROP).
   Do however note that it is not used in FANN_TRAIN_RPROP.

   The default learning rate is 0.7.

   See also:
   	<fann_set_learning_rate>, <fann_set_training_algorithm>

   This function appears in FANN >= 1.0.0.
 */
FANN_EXTERNAL float FANN_API fann_get_learning_rate(struct fann *ann);


/* Function: fann_set_learning_rate

   Set the learning rate.

   More info available in <fann_get_learning_rate>

   This function appears in FANN >= 1.0.0.
 */
FANN_EXTERNAL void FANN_API fann_set_learning_rate(struct fann *ann, float learning_rate);

/* Function: fann_get_learning_momentum

   Get the learning momentum.

   The learning momentum can be used to speed up FANN_TRAIN_INCREMENTAL training.
   A too high momentum will however not benefit training. Setting momentum to 0 will
   be the same as not using the momentum parameter. The recommended value of this parameter
   is between 0.0 and 1.0.

   The default momentum is 0.

   See also:
   <fann_set_learning_momentum>, <fann_set_training_algorithm>

   This function appears in FANN >= 2.0.0.
 */
FANN_EXTERNAL float FANN_API fann_get_learning_momentum(struct fann *ann);


/* Function: fann_set_learning_momentum

   Set the learning momentum.

   More info available in <fann_get_learning_momentum>

   This function appears in FANN >= 2.0.0.
 */
FANN_EXTERNAL void FANN_API fann_set_learning_momentum(struct fann *ann, float learning_momentum);


/* Function: fann_get_activation_function

   Get the activation function for neuron number *neuron* in layer number *layer*,
   counting the input layer as layer 0.

   It is not possible to get activation functions for the neurons in the input layer.

   Information about the individual activation functions is available at <fann_activationfunc_enum>.

   Returns:
    The activation function for the neuron or -1 if the neuron is not defined in the neural network.

   See also:
   	<fann_set_activation_function_layer>, <fann_set_activation_function_hidden>,
   	<fann_set_activation_function_output>, <fann_set_activation_steepness>,
    <fann_set_activation_function>

   This function appears in FANN >= 2.1.0
 */
FANN_EXTERNAL enum fann_activationfunc_enum FANN_API fann_get_activation_function(struct fann *ann,
																int layer,
																int neuron);

/* Function: fann_set_activation_function

   Set the activation function for neuron number *neuron* in layer number *layer*,
   counting the input layer as layer 0.

   It is not possible to set activation functions for the neurons in the input layer.

   When choosing an activation function it is important to note that the activation
   functions have different range. FANN_SIGMOID is e.g. in the 0 - 1 range while
   FANN_SIGMOID_SYMMETRIC is in the -1 - 1 range and FANN_LINEAR is unbounded.

   Information about the individual activation functions is available at <fann_activationfunc_enum>.

   The default activation function is FANN_SIGMOID_STEPWISE.

   See also:
   	<fann_set_activation_function_layer>, <fann_set_activation_function_hidden>,
   	<fann_set_activation_function_output>, <fann_set_activation_steepness>,
    <fann_get_activation_function>

   This function appears in FANN >= 2.0.0.
 */
FANN_EXTERNAL void FANN_API fann_set_activation_function(struct fann *ann,
																enum fann_activationfunc_enum
																activation_function,
																int layer,
																int neuron);

/* Function: fann_set_activation_function_layer

   Set the activation function for all the neurons in the layer number *layer*,
   counting the input layer as layer 0.

   It is not possible to set activation functions for the neurons in the input layer.

   See also:
   	<fann_set_activation_function>, <fann_set_activation_function_hidden>,
   	<fann_set_activation_function_output>, <fann_set_activation_steepness_layer>

   This function appears in FANN >= 2.0.0.
 */
FANN_EXTERNAL void FANN_API fann_set_activation_function_layer(struct fann *ann,
																enum fann_activationfunc_enum
																activation_function,
																int layer);

/* Function: fann_set_activation_function_hidden

   Set the activation function for all of the hidden layers.

   See also:
   	<fann_set_activation_function>, <fann_set_activation_function_layer>,
   	<fann_set_activation_function_output>, <fann_set_activation_steepness_hidden>

   This function appears in FANN >= 1.0.0.
 */
FANN_EXTERNAL void FANN_API fann_set_activation_function_hidden(struct fann *ann,
																enum fann_activationfunc_enum
																activation_function);


/* Function: fann_set_activation_function_output

   Set the activation function for the output layer.

   See also:
   	<fann_set_activation_function>, <fann_set_activation_function_layer>,
   	<fann_set_activation_function_hidden>, <fann_set_activation_steepness_output>

   This function appears in FANN >= 1.0.0.
 */
FANN_EXTERNAL void FANN_API fann_set_activation_function_output(struct fann *ann,
																enum fann_activationfunc_enum
																activation_function);

/* Function: fann_get_activation_steepness

   Get the activation steepness for neuron number *neuron* in layer number *layer*,
   counting the input layer as layer 0.

   It is not possible to get activation steepness for the neurons in the input layer.

   The steepness of an activation function says something about how fast the activation function
   goes from the minimum to the maximum. A high value for the activation function will also
   give a more aggressive training.

   When training neural networks where the output values should be at the extremes (usually 0 and 1,
   depending on the activation function), a steep activation function can be used (e.g. 1.0).

   The default activation steepness is 0.5.

   Returns:
    The activation steepness for the neuron or -1 if the neuron is not defined in the neural network.

   See also:
   	<fann_set_activation_steepness_layer>, <fann_set_activation_steepness_hidden>,
   	<fann_set_activation_steepness_output>, <fann_set_activation_function>,
    <fann_set_activation_steepness>

   This function appears in FANN >= 2.1.0
 */
FANN_EXTERNAL fann_type FANN_API fann_get_activation_steepness(struct fann *ann,
																int layer,
																int neuron);

/* Function: fann_set_activation_steepness

   Set the activation steepness for neuron number *neuron* in layer number *layer*,
   counting the input layer as layer 0.

   It is not possible to set activation steepness for the neurons in the input layer.

   The steepness of an activation function says something about how fast the activation function
   goes from the minimum to the maximum. A high value for the activation function will also
   give a more aggressive training.

   When training neural networks where the output values should be at the extremes (usually 0 and 1,
   depending on the activation function), a steep activation function can be used (e.g. 1.0).

   The default activation steepness is 0.5.

   See also:
   	<fann_set_activation_steepness_layer>, <fann_set_activation_steepness_hidden>,
   	<fann_set_activation_steepness_output>, <fann_set_activation_function>,
    <fann_get_activation_steepness>

   This function appears in FANN >= 2.0.0.
 */
FANN_EXTERNAL void FANN_API fann_set_activation_steepness(struct fann *ann,
																fann_type steepness,
																int layer,
																int neuron);

/* Function: fann_set_activation_steepness_layer

   Set the activation steepness for all of the neurons in layer number *layer*,
   counting the input layer as layer 0.

   It is not possible to set activation steepness for the neurons in the input layer.

   See also:
   	<fann_set_activation_steepness>, <fann_set_activation_steepness_hidden>,
   	<fann_set_activation_steepness_output>, <fann_set_activation_function_layer>

   This function appears in FANN >= 2.0.0.
 */
FANN_EXTERNAL void FANN_API fann_set_activation_steepness_layer(struct fann *ann,
																fann_type steepness,
																int layer);

/* Function: fann_set_activation_steepness_hidden

   Set the steepness of the activation steepness in all of the hidden layers.

   See also:
   	<fann_set_activation_steepness>, <fann_set_activation_steepness_layer>,
   	<fann_set_activation_steepness_output>, <fann_set_activation_function_hidden>

   This function appears in FANN >= 1.2.0.
 */
FANN_EXTERNAL void FANN_API fann_set_activation_steepness_hidden(struct fann *ann,
																 fann_type steepness);


/* Function: fann_set_activation_steepness_output

   Set the steepness of the activation steepness in the output layer.

   See also:
   	<fann_set_activation_steepness>, <fann_set_activation_steepness_layer>,
   	<fann_set_activation_steepness_hidden>, <fann_set_activation_function_output>

   This function appears in FANN >= 1.2.0.
 */
FANN_EXTERNAL void FANN_API fann_set_activation_steepness_output(struct fann *ann,
																 fann_type steepness);


/* Function: fann_get_train_error_function

   Returns the error function used during training.

   The error functions are described further in <fann_errorfunc_enum>

   The default error function is FANN_ERRORFUNC_TANH

   See also:
   	<fann_set_train_error_function>

   This function appears in FANN >= 1.2.0.
  */
FANN_EXTERNAL enum fann_errorfunc_enum FANN_API fann_get_train_error_function(struct fann *ann);


/* Function: fann_set_train_error_function

   Set the error function used during training.

   The error functions are described further in <fann_errorfunc_enum>

   See also:
   	<fann_get_train_error_function>

   This function appears in FANN >= 1.2.0.
 */
FANN_EXTERNAL void FANN_API fann_set_train_error_function(struct fann *ann,
														  enum fann_errorfunc_enum
														  train_error_function);


/* Function: fann_get_train_stop_function

   Returns the the stop function used during training.

   The stop function is described further in <fann_stopfunc_enum>

   The default stop function is FANN_STOPFUNC_MSE

   See also:
   	<fann_get_train_stop_function>, <fann_get_bit_fail_limit>

   This function appears in FANN >= 2.0.0.
 */
FANN_EXTERNAL enum fann_stopfunc_enum FANN_API fann_get_train_stop_function(struct fann *ann);


/* Function: fann_set_train_stop_function

   Set the stop function used during training.

   Returns the the stop function used during training.

   The stop function is described further in <fann_stopfunc_enum>

   See also:
   	<fann_get_train_stop_function>

   This function appears in FANN >= 2.0.0.
 */
FANN_EXTERNAL void FANN_API fann_set_train_stop_function(struct fann *ann,
														 enum fann_stopfunc_enum train_stop_function);


/* Function: fann_get_bit_fail_limit

   Returns the bit fail limit used during training.

   The bit fail limit is used during training where the <fann_stopfunc_enum> is set to FANN_STOPFUNC_BIT.

   The limit is the maximum accepted difference between the desired output and the actual output during
   training. Each output that diverges more than this limit is counted as an error bit.
   This difference is divided by two when dealing with symmetric activation functions,
   so that symmetric and not symmetric activation functions can use the same limit.

   The default bit fail limit is 0.35.

   See also:
   	<fann_set_bit_fail_limit>

   This function appears in FANN >= 2.0.0.
 */
FANN_EXTERNAL fann_type FANN_API fann_get_bit_fail_limit(struct fann *ann);

/* Function: fann_set_bit_fail_limit

   Set the bit fail limit used during training.

   See also:
   	<fann_get_bit_fail_limit>

   This function appears in FANN >= 2.0.0.
 */
FANN_EXTERNAL void FANN_API fann_set_bit_fail_limit(struct fann *ann, fann_type bit_fail_limit);

/* Function: fann_set_callback

   Sets the callback function for use during training.

   See <fann_callback_type> for more information about the callback function.

   The default callback function simply prints out some status information.

   This function appears in FANN >= 2.0.0.
 */
FANN_EXTERNAL void FANN_API fann_set_callback(struct fann *ann, fann_callback_type callback);

/* Function: fann_get_quickprop_decay

   The decay is a small negative valued number which is the factor that the weights
   should become smaller in each iteration during quickprop training. This is used
   to make sure that the weights do not become too high during training.

   The default decay is -0.0001.

   See also:
   	<fann_set_quickprop_decay>

   This function appears in FANN >= 1.2.0.
 */
FANN_EXTERNAL float FANN_API fann_get_quickprop_decay(struct fann *ann);


/* Function: fann_set_quickprop_decay

   Sets the quickprop decay factor.

   See also:
   	<fann_get_quickprop_decay>

   This function appears in FANN >= 1.2.0.
*/
FANN_EXTERNAL void FANN_API fann_set_quickprop_decay(struct fann *ann, float quickprop_decay);


/* Function: fann_get_quickprop_mu

   The mu factor is used to increase and decrease the step-size during quickprop training.
   The mu factor should always be above 1, since it would otherwise decrease the step-size
   when it was supposed to increase it.

   The default mu factor is 1.75.

   See also:
   	<fann_set_quickprop_mu>

   This function appears in FANN >= 1.2.0.
*/
FANN_EXTERNAL float FANN_API fann_get_quickprop_mu(struct fann *ann);


/* Function: fann_set_quickprop_mu

    Sets the quickprop mu factor.

   See also:
   	<fann_get_quickprop_mu>

   This function appears in FANN >= 1.2.0.
*/
FANN_EXTERNAL void FANN_API fann_set_quickprop_mu(struct fann *ann, float quickprop_mu);


/* Function: fann_get_rprop_increase_factor

   The increase factor is a value larger than 1, which is used to
   increase the step-size during RPROP training.

   The default increase factor is 1.2.

   See also:
   	<fann_set_rprop_increase_factor>

   This function appears in FANN >= 1.2.0.
*/
FANN_EXTERNAL float FANN_API fann_get_rprop_increase_factor(struct fann *ann);


/* Function: fann_set_rprop_increase_factor

   The increase factor used during RPROP training.

   See also:
   	<fann_get_rprop_increase_factor>

   This function appears in FANN >= 1.2.0.
*/
FANN_EXTERNAL void FANN_API fann_set_rprop_increase_factor(struct fann *ann,
														   float rprop_increase_factor);


/* Function: fann_get_rprop_decrease_factor

   The decrease factor is a value smaller than 1, which is used to decrease the step-size during RPROP training.

   The default decrease factor is 0.5.

   See also:
    <fann_set_rprop_decrease_factor>

   This function appears in FANN >= 1.2.0.
*/
FANN_EXTERNAL float FANN_API fann_get_rprop_decrease_factor(struct fann *ann);


/* Function: fann_set_rprop_decrease_factor

   The decrease factor is a value smaller than 1, which is used to decrease the step-size during RPROP training.

   See also:
    <fann_get_rprop_decrease_factor>

   This function appears in FANN >= 1.2.0.
*/
FANN_EXTERNAL void FANN_API fann_set_rprop_decrease_factor(struct fann *ann,
														   float rprop_decrease_factor);


/* Function: fann_get_rprop_delta_min

   The minimum step-size is a small positive number determining how small the minimum step-size may be.

   The default value delta min is 0.0.

   See also:
   	<fann_set_rprop_delta_min>

   This function appears in FANN >= 1.2.0.
*/
FANN_EXTERNAL float FANN_API fann_get_rprop_delta_min(struct fann *ann);


/* Function: fann_set_rprop_delta_min

   The minimum step-size is a small positive number determining how small the minimum step-size may be.

   See also:
   	<fann_get_rprop_delta_min>

   This function appears in FANN >= 1.2.0.
*/
FANN_EXTERNAL void FANN_API fann_set_rprop_delta_min(struct fann *ann, float rprop_delta_min);


/* Function: fann_get_rprop_delta_max

   The maximum step-size is a positive number determining how large the maximum step-size may be.

   The default delta max is 50.0.

   See also:
   	<fann_set_rprop_delta_max>, <fann_get_rprop_delta_min>

   This function appears in FANN >= 1.2.0.
*/
FANN_EXTERNAL float FANN_API fann_get_rprop_delta_max(struct fann *ann);


/* Function: fann_set_rprop_delta_max

   The maximum step-size is a positive number determining how large the maximum step-size may be.

   See also:
   	<fann_get_rprop_delta_max>, <fann_get_rprop_delta_min>

   This function appears in FANN >= 1.2.0.
*/
FANN_EXTERNAL void FANN_API fann_set_rprop_delta_max(struct fann *ann, float rprop_delta_max);

/* Function: fann_get_rprop_delta_zero

   The initial step-size is a positive number determining the initial step size.

   The default delta zero is 0.1.

   See also:
   	<fann_set_rprop_delta_zero>, <fann_get_rprop_delta_min>, <fann_get_rprop_delta_max>

   This function appears in FANN >= 2.1.0.
*/
FANN_EXTERNAL float FANN_API fann_get_rprop_delta_zero(struct fann *ann);


/* Function: fann_set_rprop_delta_zero

   The initial step-size is a positive number determining the initial step size.

   See also:
   	<fann_get_rprop_delta_zero>, <fann_get_rprop_delta_zero>

   This function appears in FANN >= 2.1.0.
*/
FANN_EXTERNAL void FANN_API fann_set_rprop_delta_zero(struct fann *ann, float rprop_delta_max);

/* Method: fann_get_sarprop_weight_decay_shift

   The sarprop weight decay shift.

   The default delta max is -6.644.

   See also:
   <fann fann_set_sarprop_weight_decay_shift>

   This function appears in FANN >= 2.1.0.
   */
FANN_EXTERNAL float FANN_API fann_get_sarprop_weight_decay_shift(struct fann *ann);

/* Method: fann_set_sarprop_weight_decay_shift

   Set the sarprop weight decay shift.

   This function appears in FANN >= 2.1.0.

   See also:
   <fann_set_sarprop_weight_decay_shift>
   */
FANN_EXTERNAL void FANN_API fann_set_sarprop_weight_decay_shift(struct fann *ann, float sarprop_weight_decay_shift);

/* Method: fann_get_sarprop_step_error_threshold_factor

   The sarprop step error threshold factor.

   The default delta max is 0.1.

   See also:
   <fann fann_get_sarprop_step_error_threshold_factor>

   This function appears in FANN >= 2.1.0.
   */
FANN_EXTERNAL float FANN_API fann_get_sarprop_step_error_threshold_factor(struct fann *ann);

/* Method: fann_set_sarprop_step_error_threshold_factor

   Set the sarprop step error threshold factor.

   This function appears in FANN >= 2.1.0.

   See also:
   <fann_get_sarprop_step_error_threshold_factor>
   */
FANN_EXTERNAL void FANN_API fann_set_sarprop_step_error_threshold_factor(struct fann *ann, float sarprop_step_error_threshold_factor);

/* Method: fann_get_sarprop_step_error_shift

   The get sarprop step error shift.

   The default delta max is 1.385.

   See also:
   <fann_set_sarprop_step_error_shift>

   This function appears in FANN >= 2.1.0.
   */
FANN_EXTERNAL float FANN_API fann_get_sarprop_step_error_shift(struct fann *ann);

/* Method: fann_set_sarprop_step_error_shift

   Set the sarprop step error shift.

   This function appears in FANN >= 2.1.0.

   See also:
   <fann_get_sarprop_step_error_shift>
   */
FANN_EXTERNAL void FANN_API fann_set_sarprop_step_error_shift(struct fann *ann, float sarprop_step_error_shift);

/* Method: fann_get_sarprop_temperature

   The sarprop weight decay shift.

   The default delta max is 0.015.

   See also:
   <fann_set_sarprop_temperature>

   This function appears in FANN >= 2.1.0.
   */
FANN_EXTERNAL float FANN_API fann_get_sarprop_temperature(struct fann *ann);

/* Method: fann_set_sarprop_temperature

   Set the sarprop_temperature.

   This function appears in FANN >= 2.1.0.

   See also:
   <fann_get_sarprop_temperature>
   */
FANN_EXTERNAL void FANN_API fann_set_sarprop_temperature(struct fann *ann, float sarprop_temperature);

#endif


/*
file https://github.com/kaizhu256/fann/blob/v2021.2.21/src/include/fann_cascade.h
*/
/*
Fast Artificial Neural Network Library (fann)
Copyright (C) 2003-2016 Steffen Nissen (steffen.fann@gmail.com)

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef __fann_cascade_h__
#define __fann_cascade_h__

/* Section: FANN Cascade Training
   Cascade training differs from ordinary training in the sense that it starts with an empty neural network
   and then adds neurons one by one, while it trains the neural network. The main benefit of this approach
   is that you do not have to guess the number of hidden layers and neurons prior to training, but cascade
   training has also proved better at solving some problems.

   The basic idea of cascade training is that a number of candidate neurons are trained separate from the
   real network, then the most promising of these candidate neurons is inserted into the neural network.
   Then the output connections are trained and new candidate neurons are prepared. The candidate neurons are
   created as shortcut connected neurons in a new hidden layer, which means that the final neural network
   will consist of a number of hidden layers with one shortcut connected neuron in each.
*/

/* Group: Cascade Training */

#ifndef FIXEDFANN
/* Function: fann_cascadetrain_on_data

   Trains on an entire dataset, for a period of time using the Cascade2 training algorithm.
   This algorithm adds neurons to the neural network while training, which means that it
   needs to start with an ANN without any hidden layers. The neural network should also use
   shortcut connections, so <fann_create_shortcut> should be used to create the ANN like this:
   >struct fann *ann = fann_create_shortcut(2, fann_num_input_train_data(train_data), fann_num_output_train_data(train_data));

   This training uses the parameters set using the fann_set_cascade_..., but it also uses another
   training algorithm as it's internal training algorithm. This algorithm can be set to either
   FANN_TRAIN_RPROP or FANN_TRAIN_QUICKPROP by <fann_set_training_algorithm>, and the parameters
   set for these training algorithms will also affect the cascade training.

   Parameters:
   		ann - The neural network
   		data - The data, which should be used during training
   		max_neuron - The maximum number of neurons to be added to neural network
   		neurons_between_reports - The number of neurons between printing a status report to stdout.
   			A value of zero means no reports should be printed.
   		desired_error - The desired <fann_get_MSE> or <fann_get_bit_fail>, depending on which stop function
   			is chosen by <fann_set_train_stop_function>.

	Instead of printing out reports every neurons_between_reports, a callback function can be called
	(see <fann_set_callback>).

	See also:
		<fann_train_on_data>, <fann_cascadetrain_on_file>, <Parameters>

	This function appears in FANN >= 2.0.0.
*/
FANN_EXTERNAL void FANN_API fann_cascadetrain_on_data(struct fann *ann,
													  struct fann_train_data *data,
													  unsigned int max_neurons,
													  unsigned int neurons_between_reports,
													  float desired_error);

/* Function: fann_cascadetrain_on_file

   Does the same as <fann_cascadetrain_on_data>, but reads the training data directly from a file.

   See also:
   		<fann_cascadetrain_on_data>

	This function appears in FANN >= 2.0.0.
*/
FANN_EXTERNAL void FANN_API fann_cascadetrain_on_file(struct fann *ann, const char *filename,
													  unsigned int max_neurons,
													  unsigned int neurons_between_reports,
													  float desired_error);

/* Group: Parameters */

/* Function: fann_get_cascade_output_change_fraction

   The cascade output change fraction is a number between 0 and 1 determining how large a fraction
   the <fann_get_MSE> value should change within <fann_get_cascade_output_stagnation_epochs> during
   training of the output connections, in order for the training not to stagnate. If the training
   stagnates, the training of the output connections will be ended and new candidates will be prepared.

   This means:
   If the MSE does not change by a fraction of <fann_get_cascade_output_change_fraction> during a
   period of <fann_get_cascade_output_stagnation_epochs>, the training of the output connections
   is stopped because the training has stagnated.

   If the cascade output change fraction is low, the output connections will be trained more and if the
   fraction is high they will be trained less.

   The default cascade output change fraction is 0.01, which is equivalent to a 1% change in MSE.

   See also:
   		<fann_set_cascade_output_change_fraction>, <fann_get_MSE>, <fann_get_cascade_output_stagnation_epochs>

	This function appears in FANN >= 2.0.0.
 */
FANN_EXTERNAL float FANN_API fann_get_cascade_output_change_fraction(struct fann *ann);


/* Function: fann_set_cascade_output_change_fraction

   Sets the cascade output change fraction.

   See also:
   		<fann_get_cascade_output_change_fraction>

	This function appears in FANN >= 2.0.0.
 */
FANN_EXTERNAL void FANN_API fann_set_cascade_output_change_fraction(struct fann *ann,
															 float cascade_output_change_fraction);

/* Function: fann_get_cascade_output_stagnation_epochs

   The number of cascade output stagnation epochs determines the number of epochs training is allowed to
   continue without changing the MSE by a fraction of <fann_get_cascade_output_change_fraction>.

   See more info about this parameter in <fann_get_cascade_output_change_fraction>.

   The default number of cascade output stagnation epochs is 12.

   See also:
   		<fann_set_cascade_output_stagnation_epochs>, <fann_get_cascade_output_change_fraction>

	This function appears in FANN >= 2.0.0.
 */
FANN_EXTERNAL unsigned int FANN_API fann_get_cascade_output_stagnation_epochs(struct fann *ann);


/* Function: fann_set_cascade_output_stagnation_epochs

   Sets the number of cascade output stagnation epochs.

   See also:
   		<fann_get_cascade_output_stagnation_epochs>

	This function appears in FANN >= 2.0.0.
 */
FANN_EXTERNAL void FANN_API fann_set_cascade_output_stagnation_epochs(struct fann *ann,
															 unsigned int cascade_output_stagnation_epochs);


/* Function: fann_get_cascade_candidate_change_fraction

   The cascade candidate change fraction is a number between 0 and 1 determining how large a fraction
   the <fann_get_MSE> value should change within <fann_get_cascade_candidate_stagnation_epochs> during
   training of the candidate neurons, in order for the training not to stagnate. If the training
   stagnates, the training of the candidate neurons will be ended and the best candidate will be selected.

   This means:
   If the MSE does not change by a fraction of <fann_get_cascade_candidate_change_fraction> during a
   period of <fann_get_cascade_candidate_stagnation_epochs>, the training of the candidate neurons
   is stopped because the training has stagnated.

   If the cascade candidate change fraction is low, the candidate neurons will be trained more and if the
   fraction is high they will be trained less.

   The default cascade candidate change fraction is 0.01, which is equivalent to a 1% change in MSE.

   See also:
   		<fann_set_cascade_candidate_change_fraction>, <fann_get_MSE>, <fann_get_cascade_candidate_stagnation_epochs>

	This function appears in FANN >= 2.0.0.
 */
FANN_EXTERNAL float FANN_API fann_get_cascade_candidate_change_fraction(struct fann *ann);


/* Function: fann_set_cascade_candidate_change_fraction

   Sets the cascade candidate change fraction.

   See also:
   		<fann_get_cascade_candidate_change_fraction>

	This function appears in FANN >= 2.0.0.
 */
FANN_EXTERNAL void FANN_API fann_set_cascade_candidate_change_fraction(struct fann *ann,
															 float cascade_candidate_change_fraction);

/* Function: fann_get_cascade_candidate_stagnation_epochs

   The number of cascade candidate stagnation epochs determines the number of epochs training is allowed to
   continue without changing the MSE by a fraction of <fann_get_cascade_candidate_change_fraction>.

   See more info about this parameter in <fann_get_cascade_candidate_change_fraction>.

   The default number of cascade candidate stagnation epochs is 12.

   See also:
   		<fann_set_cascade_candidate_stagnation_epochs>, <fann_get_cascade_candidate_change_fraction>

	This function appears in FANN >= 2.0.0.
 */
FANN_EXTERNAL unsigned int FANN_API fann_get_cascade_candidate_stagnation_epochs(struct fann *ann);


/* Function: fann_set_cascade_candidate_stagnation_epochs

   Sets the number of cascade candidate stagnation epochs.

   See also:
   		<fann_get_cascade_candidate_stagnation_epochs>

	This function appears in FANN >= 2.0.0.
 */
FANN_EXTERNAL void FANN_API fann_set_cascade_candidate_stagnation_epochs(struct fann *ann,
															 unsigned int cascade_candidate_stagnation_epochs);


/* Function: fann_get_cascade_weight_multiplier

   The weight multiplier is a parameter which is used to multiply the weights from the candidate neuron
   before adding the neuron to the neural network. This parameter is usually between 0 and 1, and is used
   to make the training a bit less aggressive.

   The default weight multiplier is 0.4

   See also:
   		<fann_set_cascade_weight_multiplier>

	This function appears in FANN >= 2.0.0.
 */
FANN_EXTERNAL fann_type FANN_API fann_get_cascade_weight_multiplier(struct fann *ann);


/* Function: fann_set_cascade_weight_multiplier

   Sets the weight multiplier.

   See also:
   		<fann_get_cascade_weight_multiplier>

	This function appears in FANN >= 2.0.0.
 */
FANN_EXTERNAL void FANN_API fann_set_cascade_weight_multiplier(struct fann *ann,
															 fann_type cascade_weight_multiplier);


/* Function: fann_get_cascade_candidate_limit

   The candidate limit is a limit for how much the candidate neuron may be trained.
   The limit is a limit on the proportion between the MSE and candidate score.

   Set this to a lower value to avoid overfitting and to a higher if overfitting is
   not a problem.

   The default candidate limit is 1000.0

   See also:
   		<fann_set_cascade_candidate_limit>

	This function appears in FANN >= 2.0.0.
 */
FANN_EXTERNAL fann_type FANN_API fann_get_cascade_candidate_limit(struct fann *ann);


/* Function: fann_set_cascade_candidate_limit

   Sets the candidate limit.

   See also:
   		<fann_get_cascade_candidate_limit>

	This function appears in FANN >= 2.0.0.
 */
FANN_EXTERNAL void FANN_API fann_set_cascade_candidate_limit(struct fann *ann,
															 fann_type cascade_candidate_limit);


/* Function: fann_get_cascade_max_out_epochs

   The maximum out epochs determines the maximum number of epochs the output connections
   may be trained after adding a new candidate neuron.

   The default max out epochs is 150

   See also:
   		<fann_set_cascade_max_out_epochs>

	This function appears in FANN >= 2.0.0.
 */
FANN_EXTERNAL unsigned int FANN_API fann_get_cascade_max_out_epochs(struct fann *ann);


/* Function: fann_set_cascade_max_out_epochs

   Sets the maximum out epochs.

   See also:
   		<fann_get_cascade_max_out_epochs>

	This function appears in FANN >= 2.0.0.
 */
FANN_EXTERNAL void FANN_API fann_set_cascade_max_out_epochs(struct fann *ann,
															 unsigned int cascade_max_out_epochs);


/* Function: fann_get_cascade_min_out_epochs

   The minimum out epochs determines the minimum number of epochs the output connections
   must be trained after adding a new candidate neuron.

   The default min out epochs is 50

   See also:
   		<fann_set_cascade_min_out_epochs>

	This function appears in FANN >= 2.2.0.
 */
FANN_EXTERNAL unsigned int FANN_API fann_get_cascade_min_out_epochs(struct fann *ann);


/* Function: fann_set_cascade_min_out_epochs

   Sets the minimum out epochs.

   See also:
   		<fann_get_cascade_min_out_epochs>

	This function appears in FANN >= 2.2.0.
 */
FANN_EXTERNAL void FANN_API fann_set_cascade_min_out_epochs(struct fann *ann,
															 unsigned int cascade_min_out_epochs);

/* Function: fann_get_cascade_max_cand_epochs

   The maximum candidate epochs determines the maximum number of epochs the input
   connections to the candidates may be trained before adding a new candidate neuron.

   The default max candidate epochs is 150

   See also:
   		<fann_set_cascade_max_cand_epochs>

	This function appears in FANN >= 2.0.0.
 */
FANN_EXTERNAL unsigned int FANN_API fann_get_cascade_max_cand_epochs(struct fann *ann);


/* Function: fann_set_cascade_max_cand_epochs

   Sets the max candidate epochs.

   See also:
   		<fann_get_cascade_max_cand_epochs>

	This function appears in FANN >= 2.0.0.
 */
FANN_EXTERNAL void FANN_API fann_set_cascade_max_cand_epochs(struct fann *ann,
															 unsigned int cascade_max_cand_epochs);


/* Function: fann_get_cascade_min_cand_epochs

   The minimum candidate epochs determines the minimum number of epochs the input
   connections to the candidates may be trained before adding a new candidate neuron.

   The default min candidate epochs is 50

   See also:
   		<fann_set_cascade_min_cand_epochs>

	This function appears in FANN >= 2.2.0.
 */
FANN_EXTERNAL unsigned int FANN_API fann_get_cascade_min_cand_epochs(struct fann *ann);


/* Function: fann_set_cascade_min_cand_epochs

   Sets the min candidate epochs.

   See also:
   		<fann_get_cascade_min_cand_epochs>

	This function appears in FANN >= 2.2.0.
 */
FANN_EXTERNAL void FANN_API fann_set_cascade_min_cand_epochs(struct fann *ann,
															 unsigned int cascade_min_cand_epochs);

/* Function: fann_get_cascade_num_candidates

   The number of candidates used during training (calculated by multiplying <fann_get_cascade_activation_functions_count>,
   <fann_get_cascade_activation_steepnesses_count> and <fann_get_cascade_num_candidate_groups>).

   The actual candidates is defined by the <fann_get_cascade_activation_functions> and
   <fann_get_cascade_activation_steepnesses> arrays. These arrays define the activation functions
   and activation steepnesses used for the candidate neurons. If there are 2 activation functions
   in the activation function array and 3 steepnesses in the steepness array, then there will be
   2x3=6 different candidates which will be trained. These 6 different candidates can be copied into
   several candidate groups, where the only difference between these groups is the initial weights.
   If the number of groups is set to 2, then the number of candidate neurons will be 2x3x2=12. The
   number of candidate groups is defined by <fann_set_cascade_num_candidate_groups>.

   The default number of candidates is 6x4x2 = 48

   See also:
   		<fann_get_cascade_activation_functions>, <fann_get_cascade_activation_functions_count>,
   		<fann_get_cascade_activation_steepnesses>, <fann_get_cascade_activation_steepnesses_count>,
   		<fann_get_cascade_num_candidate_groups>

	This function appears in FANN >= 2.0.0.
 */
FANN_EXTERNAL unsigned int FANN_API fann_get_cascade_num_candidates(struct fann *ann);

/* Function: fann_get_cascade_activation_functions_count

   The number of activation functions in the <fann_get_cascade_activation_functions> array.

   The default number of activation functions is 10.

   See also:
   		<fann_get_cascade_activation_functions>, <fann_set_cascade_activation_functions>

	This function appears in FANN >= 2.0.0.
 */
FANN_EXTERNAL unsigned int FANN_API fann_get_cascade_activation_functions_count(struct fann *ann);


/* Function: fann_get_cascade_activation_functions

   The cascade activation functions array is an array of the different activation functions used by
   the candidates.

   See <fann_get_cascade_num_candidates> for a description of which candidate neurons will be
   generated by this array.

   The default activation functions are {FANN_SIGMOID, FANN_SIGMOID_SYMMETRIC, FANN_GAUSSIAN,
   FANN_GAUSSIAN_SYMMETRIC, FANN_ELLIOT, FANN_ELLIOT_SYMMETRIC, FANN_SIN_SYMMETRIC,
   FANN_COS_SYMMETRIC, FANN_SIN, FANN_COS}

   See also:
   		<fann_get_cascade_activation_functions_count>, <fann_set_cascade_activation_functions>,
   		<fann_activationfunc_enum>

	This function appears in FANN >= 2.0.0.
 */
FANN_EXTERNAL enum fann_activationfunc_enum * FANN_API fann_get_cascade_activation_functions(
															struct fann *ann);


/* Function: fann_set_cascade_activation_functions

   Sets the array of cascade candidate activation functions. The array must be just as long
   as defined by the count.

   See <fann_get_cascade_num_candidates> for a description of which candidate neurons will be
   generated by this array.

   See also:
   		<fann_get_cascade_activation_steepnesses_count>, <fann_get_cascade_activation_steepnesses>

	This function appears in FANN >= 2.0.0.
 */
FANN_EXTERNAL void FANN_API fann_set_cascade_activation_functions(struct fann *ann,
														 enum fann_activationfunc_enum *
														 cascade_activation_functions,
														 unsigned int
														 cascade_activation_functions_count);


/* Function: fann_get_cascade_activation_steepnesses_count

   The number of activation steepnesses in the <fann_get_cascade_activation_functions> array.

   The default number of activation steepnesses is 4.

   See also:
   		<fann_get_cascade_activation_steepnesses>, <fann_set_cascade_activation_functions>

	This function appears in FANN >= 2.0.0.
 */
FANN_EXTERNAL unsigned int FANN_API fann_get_cascade_activation_steepnesses_count(struct fann *ann);


/* Function: fann_get_cascade_activation_steepnesses

   The cascade activation steepnesses array is an array of the different activation functions used by
   the candidates.

   See <fann_get_cascade_num_candidates> for a description of which candidate neurons will be
   generated by this array.

   The default activation steepnesses is {0.25, 0.50, 0.75, 1.00}

   See also:
   		<fann_set_cascade_activation_steepnesses>, <fann_get_cascade_activation_steepnesses_count>

	This function appears in FANN >= 2.0.0.
 */
FANN_EXTERNAL fann_type * FANN_API fann_get_cascade_activation_steepnesses(struct fann *ann);


/* Function: fann_set_cascade_activation_steepnesses

   Sets the array of cascade candidate activation steepnesses. The array must be just as long
   as defined by the count.

   See <fann_get_cascade_num_candidates> for a description of which candidate neurons will be
   generated by this array.

   See also:
   		<fann_get_cascade_activation_steepnesses>, <fann_get_cascade_activation_steepnesses_count>

	This function appears in FANN >= 2.0.0.
 */
FANN_EXTERNAL void FANN_API fann_set_cascade_activation_steepnesses(struct fann *ann,
														   fann_type *
														   cascade_activation_steepnesses,
														   unsigned int
														   cascade_activation_steepnesses_count);

/* Function: fann_get_cascade_num_candidate_groups

   The number of candidate groups is the number of groups of identical candidates which will be used
   during training.

   This number can be used to have more candidates without having to define new parameters for the candidates.

   See <fann_get_cascade_num_candidates> for a description of which candidate neurons will be
   generated by this parameter.

   The default number of candidate groups is 2

   See also:
   		<fann_set_cascade_num_candidate_groups>

	This function appears in FANN >= 2.0.0.
 */
FANN_EXTERNAL unsigned int FANN_API fann_get_cascade_num_candidate_groups(struct fann *ann);


/* Function: fann_set_cascade_num_candidate_groups

   Sets the number of candidate groups.

   See also:
   		<fann_get_cascade_num_candidate_groups>

	This function appears in FANN >= 2.0.0.
 */
FANN_EXTERNAL void FANN_API fann_set_cascade_num_candidate_groups(struct fann *ann,
															 unsigned int cascade_num_candidate_groups);

#endif  /* FIXEDFANN */

#endif


/*
file https://github.com/kaizhu256/fann/blob/v2021.2.21/src/include/fann_io.h
*/
/*
Fast Artificial Neural Network Library (fann)
Copyright (C) 2003-2016 Steffen Nissen (steffen.fann@gmail.com)

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef __fann_io_h__
#define __fann_io_h__

/* Section: FANN File Input/Output

   It is possible to save an entire ann to a file with <fann_save> for future loading with <fann_create_from_file>.
 */

/* Group: File Input and Output */

/* Function: fann_create_from_file

   Constructs a backpropagation neural network from a configuration file, which has been saved by <fann_save>.

   See also:
   	<fann_save>, <fann_save_to_fixed>

   This function appears in FANN >= 1.0.0.
 */
FANN_EXTERNAL struct fann *FANN_API fann_create_from_file(const char *configuration_file);


/* Function: fann_save

   Save the entire network to a configuration file.

   The configuration file contains all information about the neural network and enables
   <fann_create_from_file> to create an exact copy of the neural network and all of the
   parameters associated with the neural network.

   These three parameters (<fann_set_callback>, <fann_set_error_log>,
   <fann_set_user_data>) are *NOT* saved to the file because they cannot safely be
   ported to a different location. Also temporary parameters generated during training
   like <fann_get_MSE> are not saved.

   Return:
   The function returns 0 on success and -1 on failure.

   See also:
    <fann_create_from_file>, <fann_save_to_fixed>

   This function appears in FANN >= 1.0.0.
 */
FANN_EXTERNAL int FANN_API fann_save(struct fann *ann, const char *configuration_file);


/* Function: fann_save_to_fixed

   Saves the entire network to a configuration file.
   But it is saved in fixed point format no matter which
   format it is currently in.

   This is useful for training a network in floating points,
   and then later executing it in fixed point.

   The function returns the bit position of the fix point, which
   can be used to find out how accurate the fixed point network will be.
   A high value indicates high precision, and a low value indicates low
   precision.

   A negative value indicates very low precision, and a very
   strong possibility for overflow.
   (the actual fix point will be set to 0, since a negative
   fix point does not make sense).

   Generally, a fix point lower than 6 is bad, and should be avoided.
   The best way to avoid this, is to have less connections to each neuron,
   or just less neurons in each layer.

   The fixed point use of this network is only intended for use on machines that
   have no floating point processor, like an iPAQ. On normal computers the floating
   point version is actually faster.

   See also:
    <fann_create_from_file>, <fann_save>

   This function appears in FANN >= 1.0.0.
*/
FANN_EXTERNAL int FANN_API fann_save_to_fixed(struct fann *ann, const char *configuration_file);

#endif


/*
file https://github.com/kaizhu256/fann/blob/v2021.2.21/src/include/fann.h
*/
/*
Fast Artificial Neural Network Library (fann)
Copyright (C) 2003-2016 Steffen Nissen (steffen.fann@gmail.com)

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/* This file defines the user interface to the fann library.
   It is included from fixedfann.h, floatfann.h and doublefann.h and should
   NOT be included directly. If included directly it will react as if
   floatfann.h was included.
*/

/* Section: FANN Creation/Execution

   The FANN library is designed to be very easy to use.
   A feedforward ann can be created by a simple <fann_create_standard> function, while
   other ANNs can be created just as easily. The ANNs can be trained by <fann_train_on_file>
   and executed by <fann_run>.

   All of this can be done without much knowledge of the internals of ANNs, although the ANNs created will
   still be powerful and effective. If you have more knowledge about ANNs, and desire more control, almost
   every part of the ANNs can be parametrized to create specialized and highly optimal ANNs.
 */
/* Group: Creation, Destruction & Execution */

#ifndef FANN_INCLUDE
/* just to allow for inclusion of fann.h in normal stuations where only floats are needed */
#ifdef FIXEDFANN
// #include "fixedfann.h"
#else
// #include "floatfann.h"
#endif	/* FIXEDFANN  */

#else

/* COMPAT_TIME REPLACEMENT */
#ifndef _WIN32
#include <sys/time.h>
#else	/* _WIN32 */
#if !defined(_MSC_EXTENSIONS) && !defined(_INC_WINDOWS)
extern unsigned long __stdcall GetTickCount(void);

#else	/* _MSC_EXTENSIONS */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif	/* _MSC_EXTENSIONS */
#endif	/* _WIN32 */

#ifndef __fann_h__
#define __fann_h__

#ifdef __cplusplus
extern "C"
{

#ifndef __cplusplus
} /* to fool automatic indention engines */
#endif
#endif	/* __cplusplus */

#ifndef NULL
#define NULL 0
#endif	/* NULL */

/* ----- Macros used to define DLL external entrypoints ----- */
/*
 DLL Export, import and calling convention for Windows.
 Only defined for Microsoft VC++ FANN_EXTERNAL indicates
 that a function will be exported/imported from a dll
 FANN_API ensures that the DLL calling convention
 will be used for  a function regardless of the calling convention
 used when compiling.

 For a function to be exported from a DLL its prototype and
 declaration must be like this:
    FANN_EXTERNAL void FANN_API function(char *argument)

 The following ifdef block is a way of creating macros which
 make exporting from a DLL simple. All files within a DLL are
 compiled with the FANN_DLL_EXPORTS symbol defined on the
 command line. This symbol should not be defined on any project
 that uses this DLL. This way any other project whose source
 files include this file see FANN_EXTERNAL functions as being imported
 from a DLL, whereas a DLL sees symbols defined with this
 macro as being exported which makes calls more efficient.
 The __stdcall calling convention is used for functions in a
 windows DLL.

 The callback functions for fann_set_callback must be declared as FANN_API
 so the DLL and the application program both use the same
 calling convention.
*/

/*
 The following sets the default for MSVC++ 2003 or later to use
 the fann dll's. To use a lib or fixedfann.c, floatfann.c or doublefann.c
 with those compilers FANN_NO_DLL has to be defined before
 including the fann headers.
 The default for previous MSVC compilers such as VC++ 6 is not
 to use dll's. To use dll's FANN_USE_DLL has to be defined before
 including the fann headers.
*/
#if defined(_MSC_VER) && (_MSC_VER > 1300)
#ifndef FANN_NO_DLL
#define FANN_USE_DLL
#endif	/* FANN_USE_LIB */
#endif	/* _MSC_VER */
#if defined(_MSC_VER) && (defined(FANN_USE_DLL) || defined(FANN_DLL_EXPORTS))
#ifdef FANN_DLL_EXPORTS
#define FANN_EXTERNAL __declspec(dllexport)
#else							/*  */
#define FANN_EXTERNAL __declspec(dllimport)
#endif	/* FANN_DLL_EXPORTS*/
#define FANN_API __stdcall
#else							/*  */
#define FANN_EXTERNAL
#define FANN_API
#endif	/* _MSC_VER */
/* ----- End of macros used to define DLL external entrypoints ----- */

// #include "fann_error.h"
// #include "fann_activation.h"
// #include "fann_data.h"
// #include "fann_internal.h"
// #include "fann_train.h"
// #include "fann_cascade.h"
// #include "fann_io.h"

/* Function: fann_create_standard

	Creates a standard fully connected backpropagation neural network.

	There will be a bias neuron in each layer (except the output layer),
	and this bias neuron will be connected to all neurons in the next layer.
	When running the network, the bias nodes always emits 1.

	To destroy a <struct fann> use the <fann_destroy> function.

	Parameters:
		num_layers - The total number of layers including the input and the output layer.
		... - Integer values determining the number of neurons in each layer starting with the
			input layer and ending with the output layer.

	Returns:
		A pointer to the newly created <struct fann>.

	Example:
		> // Creating an ANN with 2 input neurons, 1 output neuron,
		> // and two hidden layers with 8 and 9 neurons
		> struct fann *ann = fann_create_standard(4, 2, 8, 9, 1);

	See also:
		<fann_create_standard_array>, <fann_create_sparse>, <fann_create_shortcut>

	This function appears in FANN >= 2.0.0.
*/
FANN_EXTERNAL struct fann *FANN_API fann_create_standard(unsigned int num_layers, ...);

/* Function: fann_create_standard_array
   Just like <fann_create_standard>, but with an array of layer sizes
   instead of individual parameters.

	Example:
		> // Creating an ANN with 2 input neurons, 1 output neuron,
		> // and two hidden layers with 8 and 9 neurons
		> unsigned int layers[4] = {2, 8, 9, 1};
		> struct fann *ann = fann_create_standard_array(4, layers);

	See also:
		<fann_create_standard>, <fann_create_sparse>, <fann_create_shortcut>

	This function appears in FANN >= 2.0.0.
*/
FANN_EXTERNAL struct fann *FANN_API fann_create_standard_array(unsigned int num_layers,
													           const unsigned int *layers);

/* Function: fann_create_sparse

	Creates a standard backpropagation neural network, which is not fully connected.

	Parameters:
		connection_rate - The connection rate controls how many connections there will be in the
   			network. If the connection rate is set to 1, the network will be fully
   			connected, but if it is set to 0.5 only half of the connections will be set.
			A connection rate of 1 will yield the same result as <fann_create_standard>
		num_layers - The total number of layers including the input and the output layer.
		... - Integer values determining the number of neurons in each layer starting with the
			input layer and ending with the output layer.

	Returns:
		A pointer to the newly created <struct fann>.

	See also:
		<fann_create_sparse_array>, <fann_create_standard>, <fann_create_shortcut>

	This function appears in FANN >= 2.0.0.
*/
FANN_EXTERNAL struct fann *FANN_API fann_create_sparse(float connection_rate,
	                                                   unsigned int num_layers, ...);


/* Function: fann_create_sparse_array
   Just like <fann_create_sparse>, but with an array of layer sizes
   instead of individual parameters.

	See <fann_create_standard_array> for a description of the parameters.

	See also:
		<fann_create_sparse>, <fann_create_standard>, <fann_create_shortcut>

	This function appears in FANN >= 2.0.0.
*/
FANN_EXTERNAL struct fann *FANN_API fann_create_sparse_array(float connection_rate,
	                                                         unsigned int num_layers,
															 const unsigned int *layers);

/* Function: fann_create_shortcut

	Creates a standard backpropagation neural network, which is fully connected and which
	also has shortcut connections.

 	Shortcut connections are connections that skip layers. A fully connected network with shortcut
	connections is a network where all neurons are connected to all neurons in later layers.
	Including direct connections from the input layer to the output layer.

	See <fann_create_standard> for a description of the parameters.

	See also:
		<fann_create_shortcut_array>, <fann_create_standard>, <fann_create_sparse>,

	This function appears in FANN >= 2.0.0.
*/
FANN_EXTERNAL struct fann *FANN_API fann_create_shortcut(unsigned int num_layers, ...);

/* Function: fann_create_shortcut_array
   Just like <fann_create_shortcut>, but with an array of layer sizes
   instead of individual parameters.

	See <fann_create_standard_array> for a description of the parameters.

	See also:
		<fann_create_shortcut>, <fann_create_standard>, <fann_create_sparse>

	This function appears in FANN >= 2.0.0.
*/
FANN_EXTERNAL struct fann *FANN_API fann_create_shortcut_array(unsigned int num_layers,
															   const unsigned int *layers);
/* Function: fann_destroy
   Destroys the entire network, properly freeing all the associated memory.

	This function appears in FANN >= 1.0.0.
*/
FANN_EXTERNAL void FANN_API fann_destroy(struct fann *ann);


/* Function: fann_copy
   Creates a copy of a fann structure.

   Data in the user data <fann_set_user_data> is not copied, but the user data pointer is copied.

	This function appears in FANN >= 2.2.0.
*/
FANN_EXTERNAL struct fann * FANN_API fann_copy(struct fann *ann);


/* Function: fann_run
	Will run input through the neural network, returning an array of outputs, the number of which being
	equal to the number of neurons in the output layer.

	See also:
		<fann_test>

	This function appears in FANN >= 1.0.0.
*/
FANN_EXTERNAL fann_type * FANN_API fann_run(struct fann *ann, fann_type * input);

/* Function: fann_randomize_weights
	Give each connection a random weight between *min_weight* and *max_weight*

	From the beginning the weights are random between -0.1 and 0.1.

	See also:
		<fann_init_weights>

	This function appears in FANN >= 1.0.0.
*/
FANN_EXTERNAL void FANN_API fann_randomize_weights(struct fann *ann, fann_type min_weight,
												   fann_type max_weight);

/* Function: fann_init_weights
  	Initialize the weights using Widrow + Nguyen's algorithm.

 	This function behaves similarly to fann_randomize_weights. It will use the algorithm developed
	by Derrick Nguyen and Bernard Widrow to set the weights in such a way
	as to speed up training. This technique is not always successful, and in some cases can be less
	efficient than a purely random initialization.

	The algorithm requires access to the range of the input data (ie, largest and smallest input),
	and therefore accepts a second argument, data, which is the training data that will be used to
	train the network.

	See also:
		<fann_randomize_weights>, <fann_read_train_from_file>

	This function appears in FANN >= 1.1.0.
*/
FANN_EXTERNAL void FANN_API fann_init_weights(struct fann *ann, struct fann_train_data *train_data);

/* Function: fann_print_connections
	Will print the connections of the ann in a compact matrix, for easy viewing of the internals
	of the ann.

	The output from fann_print_connections on a small (2 2 1) network trained on the xor problem
	>Layer / Neuron 012345
	>L   1 / N    3 BBa...
	>L   1 / N    4 BBA...
	>L   1 / N    5 ......
	>L   2 / N    6 ...BBA
	>L   2 / N    7 ......

	This network has five real neurons and two bias neurons. This gives a total of seven neurons
	named from 0 to 6. The connections between these neurons can be seen in the matrix. "." is a
	place where there is no connection, while a character tells how strong the connection is on a
	scale from a-z. The two real neurons in the hidden layer (neuron 3 and 4 in layer 1) have
	connections from the three neurons in the previous layer as is visible in the first two lines.
	The output neuron (6) has connections from the three neurons in the hidden layer 3 - 5 as is
	visible in the fourth line.

	To simplify the matrix output neurons are not visible as neurons that connections can come from,
	and input and bias neurons are not visible as neurons that connections can go to.

	This function appears in FANN >= 1.2.0.
*/
FANN_EXTERNAL void FANN_API fann_print_connections(struct fann *ann);

/* Group: Parameters */
/* Function: fann_print_parameters

  	Prints all of the parameters and options of the ANN

	This function appears in FANN >= 1.2.0.
*/
FANN_EXTERNAL void FANN_API fann_print_parameters(struct fann *ann);


/* Function: fann_get_num_input

   Get the number of input neurons.

	This function appears in FANN >= 1.0.0.
*/
FANN_EXTERNAL unsigned int FANN_API fann_get_num_input(struct fann *ann);


/* Function: fann_get_num_output

   Get the number of output neurons.

	This function appears in FANN >= 1.0.0.
*/
FANN_EXTERNAL unsigned int FANN_API fann_get_num_output(struct fann *ann);


/* Function: fann_get_total_neurons

   Get the total number of neurons in the entire network. This number does also include the
	bias neurons, so a 2-4-2 network has 2+4+2 +2(bias) = 10 neurons.

	This function appears in FANN >= 1.0.0.
*/
FANN_EXTERNAL unsigned int FANN_API fann_get_total_neurons(struct fann *ann);


/* Function: fann_get_total_connections

   Get the total number of connections in the entire network.

	This function appears in FANN >= 1.0.0.
*/
FANN_EXTERNAL unsigned int FANN_API fann_get_total_connections(struct fann *ann);

/* Function: fann_get_network_type

    Get the type of neural network it was created as.

    Parameters:
		ann - A previously created neural network structure of
            type <struct fann> pointer.

	Returns:
        The neural network type from enum <fann_network_type_enum>

    See Also:
        <fann_network_type_enum>

   This function appears in FANN >= 2.1.0
*/
FANN_EXTERNAL enum fann_nettype_enum FANN_API fann_get_network_type(struct fann *ann);

/* Function: fann_get_connection_rate

    Get the connection rate used when the network was created

    Parameters:
		ann - A previously created neural network structure of
            type <struct fann> pointer.

	Returns:
        The connection rate

   This function appears in FANN >= 2.1.0
*/
FANN_EXTERNAL float FANN_API fann_get_connection_rate(struct fann *ann);

/* Function: fann_get_num_layers

    Get the number of layers in the network

    Parameters:
		ann - A previously created neural network structure of
            type <struct fann> pointer.

	Returns:
		The number of layers in the neural network

	Example:
		> // Obtain the number of layers in a neural network
		> struct fann *ann = fann_create_standard(4, 2, 8, 9, 1);
        > unsigned int num_layers = fann_get_num_layers(ann);

   This function appears in FANN >= 2.1.0
*/
FANN_EXTERNAL unsigned int FANN_API fann_get_num_layers(struct fann *ann);

/*Function: fann_get_layer_array

    Get the number of neurons in each layer in the network.

    Bias is not included so the layers match the fann_create functions.

    Parameters:
		ann - A previously created neural network structure of
            type <struct fann> pointer.

    The layers array must be preallocated to at least
    sizeof(unsigned int) * fann_num_layers() long.

   This function appears in FANN >= 2.1.0
*/
FANN_EXTERNAL void FANN_API fann_get_layer_array(struct fann *ann, unsigned int *layers);

/* Function: fann_get_bias_array

    Get the number of bias in each layer in the network.

    Parameters:
		ann - A previously created neural network structure of
            type <struct fann> pointer.

    The bias array must be preallocated to at least
    sizeof(unsigned int) * fann_num_layers() long.

   This function appears in FANN >= 2.1.0
*/
FANN_EXTERNAL void FANN_API fann_get_bias_array(struct fann *ann, unsigned int *bias);

/* Function: fann_get_connection_array

    Get the connections in the network.

    Parameters:
		ann - A previously created neural network structure of
            type <struct fann> pointer.

    The connections array must be preallocated to at least
    sizeof(struct fann_connection) * fann_get_total_connections() long.

   This function appears in FANN >= 2.1.0
*/
FANN_EXTERNAL void FANN_API fann_get_connection_array(struct fann *ann,
    struct fann_connection *connections);

/* Function: fann_set_weight_array

    Set connections in the network.

    Parameters:
		ann - A previously created neural network structure of
            type <struct fann> pointer.

    Only the weights can be changed, connections and weights are ignored
    if they do not already exist in the network.

    The array must have sizeof(struct fann_connection) * num_connections size.

   This function appears in FANN >= 2.1.0
*/
FANN_EXTERNAL void FANN_API fann_set_weight_array(struct fann *ann,
    struct fann_connection *connections, unsigned int num_connections);

/* Function: fann_set_weight

    Set a connection in the network.

    Parameters:
		ann - A previously created neural network structure of
            type <struct fann> pointer.

    Only the weights can be changed. The connection/weight is
    ignored if it does not already exist in the network.

   This function appears in FANN >= 2.1.0
*/
FANN_EXTERNAL void FANN_API fann_set_weight(struct fann *ann,
    unsigned int from_neuron, unsigned int to_neuron, fann_type weight);

/* Function: fann_get_weights

    Get all the network weights.

    Parameters:
		ann - A previously created neural network structure of
            type <struct fann> pointer.
		weights - A fann_type pointer to user data. It is the responsibility
			of the user to allocate sufficient space to store all the weights.

   This function appears in FANN >= x.y.z
*/
FANN_EXTERNAL void FANN_API fann_get_weights(struct fann *ann, fann_type *weights);


/* Function: fann_set_weights

    Set network weights.

    Parameters:
		ann - A previously created neural network structure of
            type <struct fann> pointer.
		weights - A fann_type pointer to user data. It is the responsibility
			of the user to make the weights array sufficient long
			to store all the weights.

   This function appears in FANN >= x.y.z
*/
FANN_EXTERNAL void FANN_API fann_set_weights(struct fann *ann, fann_type *weights);


/* Function: fann_set_user_data

    Store a pointer to user defined data. The pointer can be
    retrieved with <fann_get_user_data> for example in a
    callback. It is the user's responsibility to allocate and
    deallocate any data that the pointer might point to.

    Parameters:
		ann - A previously created neural network structure of
            type <struct fann> pointer.
		user_data - A void pointer to user defined data.

   This function appears in FANN >= 2.1.0
*/
FANN_EXTERNAL void FANN_API fann_set_user_data(struct fann *ann, void *user_data);

/* Function: fann_get_user_data

    Get a pointer to user defined data that was previously set
    with <fann_set_user_data>. It is the user's responsibility to
    allocate and deallocate any data that the pointer might point to.

    Parameters:
		ann - A previously created neural network structure of
            type <struct fann> pointer.

    Returns:
        A void pointer to user defined data.

   This function appears in FANN >= 2.1.0
*/
FANN_EXTERNAL void * FANN_API fann_get_user_data(struct fann *ann);

/* Function: fann_disable_seed_rand

   Disables the automatic random generator seeding that happens in FANN.

   Per default FANN will always seed the random generator when creating a new network,
   unless FANN_NO_SEED is defined during compilation of the library. This method can
   disable this at runtime.

   This function appears in FANN >= 2.3.0
*/
FANN_EXTERNAL void FANN_API fann_disable_seed_rand();

/* Function: fann_enable_seed_rand

   Enables the automatic random generator seeding that happens in FANN.

   Per default FANN will always seed the random generator when creating a new network,
   unless FANN_NO_SEED is defined during compilation of the library. This method can
   disable this at runtime.

   This function appears in FANN >= 2.3.0
*/
FANN_EXTERNAL void FANN_API fann_enable_seed_rand();


#ifdef FIXEDFANN

/* Function: fann_get_decimal_point

	Returns the position of the decimal point in the ann.

	This function is only available when the ANN is in fixed point mode.

	The decimal point is described in greater detail in the tutorial <Fixed Point Usage>.

	See also:
		<Fixed Point Usage>, <fann_get_multiplier>, <fann_save_to_fixed>, <fann_save_train_to_fixed>

	This function appears in FANN >= 1.0.0.
*/
FANN_EXTERNAL unsigned int FANN_API fann_get_decimal_point(struct fann *ann);


/* Function: fann_get_multiplier

    returns the multiplier that fix point data is multiplied with.

	This function is only available when the ANN is in fixed point mode.

	The multiplier is the used to convert between floating point and fixed point notation.
	A floating point number is multiplied with the multiplier in order to get the fixed point
	number and visa versa.

	The multiplier is described in greater detail in the tutorial <Fixed Point Usage>.

	See also:
		<Fixed Point Usage>, <fann_get_decimal_point>, <fann_save_to_fixed>, <fann_save_train_to_fixed>

	This function appears in FANN >= 1.0.0.
*/
FANN_EXTERNAL unsigned int FANN_API fann_get_multiplier(struct fann *ann);

#endif	/* FIXEDFANN */

#ifdef __cplusplus
#ifndef __cplusplus
/* to fool automatic indention engines */
{

#endif
}
#endif	/* __cplusplus */

#endif	/* __fann_h__ */

#endif /* NOT FANN_INCLUDE */


/*
file https://github.com/kaizhu256/fann/blob/v2021.2.21/src/include/parallel_fann.h
*/
/*
 * parallel_FANN.h
 *
 *     Author: Alessandro Pietro Bardelli
 */
#ifndef DISABLE_PARALLEL_FANN
#ifndef PARALLEL_FANN_H_
#define PARALLEL_FANN_H_

// #include "fann.h"

#ifdef __cplusplus
extern "C"
{

#ifndef __cplusplus
} /* to fool automatic indention engines */
#endif
#endif	/* __cplusplus */

#ifndef FIXEDFANN
FANN_EXTERNAL float FANN_API fann_train_epoch_batch_parallel(struct fann *ann, struct fann_train_data *data, const unsigned int threadnumb);

FANN_EXTERNAL float FANN_API fann_train_epoch_irpropm_parallel(struct fann *ann, struct fann_train_data *data, const unsigned int threadnumb);

FANN_EXTERNAL float FANN_API fann_train_epoch_quickprop_parallel(struct fann *ann, struct fann_train_data *data, const unsigned int threadnumb);

FANN_EXTERNAL float FANN_API fann_train_epoch_sarprop_parallel(struct fann *ann, struct fann_train_data *data, const unsigned int threadnumb);

FANN_EXTERNAL float FANN_API fann_train_epoch_incremental_mod(struct fann *ann, struct fann_train_data *data);

FANN_EXTERNAL float FANN_API fann_test_data_parallel(struct fann *ann, struct fann_train_data *data, const unsigned int threadnumb);
#endif /* FIXEDFANN */

#ifdef __cplusplus
#ifndef __cplusplus
/* to fool automatic indention engines */
{

#endif
}
#endif	/* __cplusplus */

#endif /* PARALLEL_FANN_H_ */
#endif /* DISABLE_PARALLEL_FANN */


/*
file https://github.com/kaizhu256/fann/blob/v2021.2.21/src/fann.c
*/
/*
  Fast Artificial Neural Network Library (fann)
  Copyright (C) 2003-2016 Steffen Nissen (steffen.fann@gmail.com)

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <math.h>

// #include "config.h"
// #include "fann.h"

/* #define FANN_NO_SEED */

FANN_EXTERNAL struct fann *FANN_API fann_create_standard(unsigned int num_layers, ...)
{
	struct fann *ann;
	va_list layer_sizes;
	int i;
	int status;
	int arg;
	unsigned int *layers = (unsigned int *) calloc(num_layers, sizeof(unsigned int));

	if(layers == NULL)
	{
		fann_error(NULL, FANN_E_CANT_ALLOCATE_MEM);
		return NULL;
	}

	va_start(layer_sizes, num_layers);

	status = 1;
	for(i = 0; i < (int) num_layers; i++)
	{
		arg = va_arg(layer_sizes, unsigned int);
		if(arg < 0 || arg > 1000000)
			status = 0;
		layers[i] = arg;
	}
	va_end(layer_sizes);

	if(!status)
	{
		fann_error(NULL, FANN_E_CANT_ALLOCATE_MEM);
		free(layers);
		return NULL;
	}

	ann = fann_create_standard_array(num_layers, layers);

	free(layers);

	return ann;
}

FANN_EXTERNAL struct fann *FANN_API fann_create_standard_array(unsigned int num_layers,
															   const unsigned int *layers)
{
	return fann_create_sparse_array(1, num_layers, layers);
}

FANN_EXTERNAL struct fann *FANN_API fann_create_sparse(float connection_rate,
													   unsigned int num_layers, ...)
{
	struct fann *ann;
	va_list layer_sizes;
	int i;
	int status;
	int arg;
	unsigned int *layers = (unsigned int *) calloc(num_layers, sizeof(unsigned int));

	if(layers == NULL)
	{
		fann_error(NULL, FANN_E_CANT_ALLOCATE_MEM);
		return NULL;
	}

	va_start(layer_sizes, num_layers);
	status = 1;
	for(i = 0; i < (int) num_layers; i++)
	{
		arg = va_arg(layer_sizes, unsigned int);
		if(arg < 0 || arg > 1000000)
			status = 0;
		layers[i] = arg;
	}
	va_end(layer_sizes);

	if(!status)
	{
		fann_error(NULL, FANN_E_CANT_ALLOCATE_MEM);
		free(layers);
		return NULL;
	}

	ann = fann_create_sparse_array(connection_rate, num_layers, layers);
	free(layers);

	return ann;
}

FANN_EXTERNAL struct fann *FANN_API fann_create_sparse_array(float connection_rate,
															 unsigned int num_layers,
															 const unsigned int *layers)
{
	struct fann_layer *layer_it, *last_layer, *prev_layer;
	struct fann *ann;
	struct fann_neuron *neuron_it, *last_neuron, *random_neuron, *bias_neuron;
#ifdef DEBUG
	unsigned int prev_layer_size;
#endif
	unsigned int num_neurons_in, num_neurons_out, i, j;
	unsigned int min_connections, max_connections, num_connections;
	unsigned int connections_per_neuron, allocated_connections;
	unsigned int random_number, found_connection, tmp_con;

#ifdef FIXEDFANN
	unsigned int multiplier;
#endif
	if(connection_rate > 1)
	{
		connection_rate = 1;
	}

	fann_seed_rand();

	/* allocate the general structure */
	ann = fann_allocate_structure(num_layers);
	if(ann == NULL)
	{
		fann_error(NULL, FANN_E_CANT_ALLOCATE_MEM);
		return NULL;
	}

	ann->connection_rate = connection_rate;
#ifdef FIXEDFANN
	multiplier = ann->multiplier;
	fann_update_stepwise(ann);
#endif

	/* determine how many neurons there should be in each layer */
	i = 0;
	for(layer_it = ann->first_layer; layer_it != ann->last_layer; layer_it++)
	{
		/* we do not allocate room here, but we make sure that
		 * last_neuron - first_neuron is the number of neurons */
		layer_it->first_neuron = NULL;
		layer_it->last_neuron = layer_it->first_neuron + layers[i++] + 1;	/* +1 for bias */
		ann->total_neurons += (unsigned int)(layer_it->last_neuron - layer_it->first_neuron);
	}

	ann->num_output = (unsigned int)((ann->last_layer - 1)->last_neuron - (ann->last_layer - 1)->first_neuron - 1);
	ann->num_input = (unsigned int)(ann->first_layer->last_neuron - ann->first_layer->first_neuron - 1);

	/* allocate room for the actual neurons */
	fann_allocate_neurons(ann);
	if(ann->errno_f == FANN_E_CANT_ALLOCATE_MEM)
	{
		fann_destroy(ann);
		return NULL;
	}

#ifdef DEBUG
	printf("creating network with connection rate %f\n", connection_rate);
	printf("input\n");
	printf("  layer       : %d neurons, 1 bias\n",
		   (int)(ann->first_layer->last_neuron - ann->first_layer->first_neuron - 1));
#endif

	num_neurons_in = ann->num_input;
	for(layer_it = ann->first_layer + 1; layer_it != ann->last_layer; layer_it++)
	{
		num_neurons_out = (unsigned int)(layer_it->last_neuron - layer_it->first_neuron - 1);
		/*�if all neurons in each layer should be connected to at least one neuron
		 * in the previous layer, and one neuron in the next layer.
		 * and the bias node should be connected to the all neurons in the next layer.
		 * Then this is the minimum amount of neurons */
		min_connections = fann_max(num_neurons_in, num_neurons_out); /* not calculating bias */
		max_connections = num_neurons_in * num_neurons_out;	     /* not calculating bias */
		num_connections = fann_max(min_connections,
								   (unsigned int) (0.5 + (connection_rate * max_connections))) +
								   num_neurons_out;

		connections_per_neuron = num_connections / num_neurons_out;
		allocated_connections = 0;
		/* Now split out the connections on the different neurons */
		for(i = 0; i != num_neurons_out; i++)
		{
			layer_it->first_neuron[i].first_con = ann->total_connections + allocated_connections;
			allocated_connections += connections_per_neuron;
			layer_it->first_neuron[i].last_con = ann->total_connections + allocated_connections;

			layer_it->first_neuron[i].activation_function = FANN_SIGMOID_STEPWISE;
#ifdef FIXEDFANN
			layer_it->first_neuron[i].activation_steepness = ann->multiplier / 2;
#else
			layer_it->first_neuron[i].activation_steepness = 0.5;
#endif

			if(allocated_connections < (num_connections * (i + 1)) / num_neurons_out)
			{
				layer_it->first_neuron[i].last_con++;
				allocated_connections++;
			}
		}

		/* bias neuron also gets stuff */
		layer_it->first_neuron[i].first_con = ann->total_connections + allocated_connections;
		layer_it->first_neuron[i].last_con = ann->total_connections + allocated_connections;

		ann->total_connections += num_connections;

		/* used in the next run of the loop */
		num_neurons_in = num_neurons_out;
	}

	fann_allocate_connections(ann);
	if(ann->errno_f == FANN_E_CANT_ALLOCATE_MEM)
	{
		fann_destroy(ann);
		return NULL;
	}

	if(connection_rate >= 1)
	{
#ifdef DEBUG
		prev_layer_size = ann->num_input + 1;
#endif
		prev_layer = ann->first_layer;
		last_layer = ann->last_layer;
		for(layer_it = ann->first_layer + 1; layer_it != last_layer; layer_it++)
		{
			last_neuron = layer_it->last_neuron - 1;
			for(neuron_it = layer_it->first_neuron; neuron_it != last_neuron; neuron_it++)
			{
				tmp_con = neuron_it->last_con - 1;
				for(i = neuron_it->first_con; i != tmp_con; i++)
				{
					ann->weights[i] = (fann_type) fann_random_weight();
					/* these connections are still initialized for fully connected networks, to allow
					 * operations to work, that are not optimized for fully connected networks.
					 */
					ann->connections[i] = prev_layer->first_neuron + (i - neuron_it->first_con);
				}

				/* bias weight */
				ann->weights[tmp_con] = (fann_type) fann_random_bias_weight();
				ann->connections[tmp_con] = prev_layer->first_neuron + (tmp_con - neuron_it->first_con);
			}
#ifdef DEBUG
			prev_layer_size = layer_it->last_neuron - layer_it->first_neuron;
#endif
			prev_layer = layer_it;
#ifdef DEBUG
			printf("  layer       : %d neurons, 1 bias\n", prev_layer_size - 1);
#endif
		}
	}
	else
	{
		/* make connections for a network, that are not fully connected */

		/* generally, what we do is first to connect all the input
		 * neurons to a output neuron, respecting the number of
		 * available input neurons for each output neuron. Then
		 * we go through all the output neurons, and connect the
		 * rest of the connections to input neurons, that they are
		 * not allready connected to.
		 */

		/* All the connections are cleared by calloc, because we want to
		 * be able to see which connections are allready connected */

		for(layer_it = ann->first_layer + 1; layer_it != ann->last_layer; layer_it++)
		{

			num_neurons_out = (unsigned int)(layer_it->last_neuron - layer_it->first_neuron - 1);
			num_neurons_in = (unsigned int)((layer_it - 1)->last_neuron - (layer_it - 1)->first_neuron - 1);

			/* first connect the bias neuron */
			bias_neuron = (layer_it - 1)->last_neuron - 1;
			last_neuron = layer_it->last_neuron - 1;
			for(neuron_it = layer_it->first_neuron; neuron_it != last_neuron; neuron_it++)
			{

				ann->connections[neuron_it->first_con] = bias_neuron;
				ann->weights[neuron_it->first_con] = (fann_type) fann_random_bias_weight();
			}

			/* then connect all neurons in the input layer */
			last_neuron = (layer_it - 1)->last_neuron - 1;
			for(neuron_it = (layer_it - 1)->first_neuron; neuron_it != last_neuron; neuron_it++)
			{

				/* random neuron in the output layer that has space
				 * for more connections */
				do
				{
					random_number = (int) (0.5 + fann_rand(0, num_neurons_out - 1));
					random_neuron = layer_it->first_neuron + random_number;
					/* checks the last space in the connections array for room */
				}
				while(ann->connections[random_neuron->last_con - 1]);

				/* find an empty space in the connection array and connect */
				for(i = random_neuron->first_con; i < random_neuron->last_con; i++)
				{
					if(ann->connections[i] == NULL)
					{
						ann->connections[i] = neuron_it;
						ann->weights[i] = (fann_type) fann_random_weight();
						break;
					}
				}
			}

			/* then connect the rest of the unconnected neurons */
			last_neuron = layer_it->last_neuron - 1;
			for(neuron_it = layer_it->first_neuron; neuron_it != last_neuron; neuron_it++)
			{
				/* find empty space in the connection array and connect */
				for(i = neuron_it->first_con; i < neuron_it->last_con; i++)
				{
					/* continue if allready connected */
					if(ann->connections[i] != NULL)
						continue;

					do
					{
						found_connection = 0;
						random_number = (int) (0.5 + fann_rand(0, num_neurons_in - 1));
						random_neuron = (layer_it - 1)->first_neuron + random_number;

						/* check to see if this connection is allready there */
						for(j = neuron_it->first_con; j < i; j++)
						{
							if(random_neuron == ann->connections[j])
							{
								found_connection = 1;
								break;
							}
						}

					}
					while(found_connection);

					/* we have found a neuron that is not allready
					 * connected to us, connect it */
					ann->connections[i] = random_neuron;
					ann->weights[i] = (fann_type) fann_random_weight();
				}
			}

#ifdef DEBUG
			printf("  layer       : %d neurons, 1 bias\n", num_neurons_out);
#endif
		}

		/* TODO it would be nice to have the randomly created
		 * connections sorted for smoother memory access.
		 */
	}

#ifdef DEBUG
	printf("output\n");
#endif

	return ann;
}


FANN_EXTERNAL struct fann *FANN_API fann_create_shortcut(unsigned int num_layers, ...)
{
	struct fann *ann;
	int i;
	int status;
	int arg;
	va_list layer_sizes;
	unsigned int *layers = (unsigned int *) calloc(num_layers, sizeof(unsigned int));

	if(layers == NULL)
	{
		fann_error(NULL, FANN_E_CANT_ALLOCATE_MEM);
		return NULL;
	}

	va_start(layer_sizes, num_layers);
	status = 1;
	for(i = 0; i < (int) num_layers; i++)
	{
		arg = va_arg(layer_sizes, unsigned int);
		if(arg < 0 || arg > 1000000)
			status = 0;
		layers[i] = arg;
	}
	va_end(layer_sizes);

	if(!status)
	{
		fann_error(NULL, FANN_E_CANT_ALLOCATE_MEM);
		free(layers);
		return NULL;
	}

	ann = fann_create_shortcut_array(num_layers, layers);

	free(layers);

	return ann;
}

FANN_EXTERNAL struct fann *FANN_API fann_create_shortcut_array(unsigned int num_layers,
															   const unsigned int *layers)
{
	struct fann_layer *layer_it, *layer_it2, *last_layer;
	struct fann *ann;
	struct fann_neuron *neuron_it, *neuron_it2 = 0;
	unsigned int i;
	unsigned int num_neurons_in, num_neurons_out;

#ifdef FIXEDFANN
	unsigned int multiplier;
#endif
	fann_seed_rand();

	/* allocate the general structure */
	ann = fann_allocate_structure(num_layers);
	if(ann == NULL)
	{
		fann_error(NULL, FANN_E_CANT_ALLOCATE_MEM);
		return NULL;
	}

	ann->connection_rate = 1;
	ann->network_type = FANN_NETTYPE_SHORTCUT;
#ifdef FIXEDFANN
	multiplier = ann->multiplier;
	fann_update_stepwise(ann);
#endif

	/* determine how many neurons there should be in each layer */
	i = 0;
	for(layer_it = ann->first_layer; layer_it != ann->last_layer; layer_it++)
	{
		/* we do not allocate room here, but we make sure that
		 * last_neuron - first_neuron is the number of neurons */
		layer_it->first_neuron = NULL;
		layer_it->last_neuron = layer_it->first_neuron + layers[i++];
		if(layer_it == ann->first_layer)
		{
			/* there is a bias neuron in the first layer */
			layer_it->last_neuron++;
		}

		ann->total_neurons += (unsigned int)(layer_it->last_neuron - layer_it->first_neuron);
	}

	ann->num_output = (unsigned int)((ann->last_layer - 1)->last_neuron - (ann->last_layer - 1)->first_neuron);
	ann->num_input = (unsigned int)(ann->first_layer->last_neuron - ann->first_layer->first_neuron - 1);

	/* allocate room for the actual neurons */
	fann_allocate_neurons(ann);
	if(ann->errno_f == FANN_E_CANT_ALLOCATE_MEM)
	{
		fann_destroy(ann);
		return NULL;
	}

#ifdef DEBUG
	printf("creating fully shortcut connected network.\n");
	printf("input\n");
	printf("  layer       : %d neurons, 1 bias\n",
		   (int)(ann->first_layer->last_neuron - ann->first_layer->first_neuron - 1));
#endif

	num_neurons_in = ann->num_input;
	last_layer = ann->last_layer;
	for(layer_it = ann->first_layer + 1; layer_it != last_layer; layer_it++)
	{
		num_neurons_out = (unsigned int)(layer_it->last_neuron - layer_it->first_neuron);

		/* Now split out the connections on the different neurons */
		for(i = 0; i != num_neurons_out; i++)
		{
			layer_it->first_neuron[i].first_con = ann->total_connections;
			ann->total_connections += num_neurons_in + 1;
			layer_it->first_neuron[i].last_con = ann->total_connections;

			layer_it->first_neuron[i].activation_function = FANN_SIGMOID_STEPWISE;
#ifdef FIXEDFANN
			layer_it->first_neuron[i].activation_steepness = ann->multiplier / 2;
#else
			layer_it->first_neuron[i].activation_steepness = 0.5;
#endif
		}

#ifdef DEBUG
		printf("  layer       : %d neurons, 0 bias\n", num_neurons_out);
#endif
		/* used in the next run of the loop */
		num_neurons_in += num_neurons_out;
	}

	fann_allocate_connections(ann);
	if(ann->errno_f == FANN_E_CANT_ALLOCATE_MEM)
	{
		fann_destroy(ann);
		return NULL;
	}

	/* Connections are created from all neurons to all neurons in later layers
	 */
	num_neurons_in = ann->num_input + 1;
	for(layer_it = ann->first_layer + 1; layer_it != last_layer; layer_it++)
	{
		for(neuron_it = layer_it->first_neuron; neuron_it != layer_it->last_neuron; neuron_it++)
		{

			i = neuron_it->first_con;
			for(layer_it2 = ann->first_layer; layer_it2 != layer_it; layer_it2++)
			{
				for(neuron_it2 = layer_it2->first_neuron; neuron_it2 != layer_it2->last_neuron;
					neuron_it2++)
				{

					ann->weights[i] = (fann_type) fann_random_weight();
					ann->connections[i] = neuron_it2;
					i++;
				}
			}
		}
		num_neurons_in += (unsigned int)(layer_it->last_neuron - layer_it->first_neuron);
	}

#ifdef DEBUG
	printf("output\n");
#endif

	return ann;
}

FANN_EXTERNAL fann_type *FANN_API fann_run(struct fann * ann, fann_type * input)
{
	struct fann_neuron *neuron_it, *last_neuron, *neurons, **neuron_pointers;
	unsigned int i, num_connections, num_input, num_output;
	fann_type neuron_sum, *output;
	fann_type *weights;
	struct fann_layer *layer_it, *last_layer;
	unsigned int activation_function;
	fann_type steepness;

	/* store some variabels local for fast access */
	struct fann_neuron *first_neuron = ann->first_layer->first_neuron;

#ifdef FIXEDFANN
	int multiplier = ann->multiplier;
	unsigned int decimal_point = ann->decimal_point;

	/* values used for the stepwise linear sigmoid function */
	fann_type r1 = 0, r2 = 0, r3 = 0, r4 = 0, r5 = 0, r6 = 0;
	fann_type v1 = 0, v2 = 0, v3 = 0, v4 = 0, v5 = 0, v6 = 0;

	fann_type last_steepness = 0;
	unsigned int last_activation_function = 0;
#else
	fann_type max_sum = 0;
#endif

	/* first set the input */
	num_input = ann->num_input;
	for(i = 0; i != num_input; i++)
	{
#ifdef FIXEDFANN
		if(fann_abs(input[i]) > multiplier)
		{
			printf
				("Warning input number %d is out of range -%d - %d with value %d, integer overflow may occur.\n",
				 i, multiplier, multiplier, input[i]);
		}
#endif
		first_neuron[i].value = input[i];
	}
	/* Set the bias neuron in the input layer */
#ifdef FIXEDFANN
	(ann->first_layer->last_neuron - 1)->value = multiplier;
#else
	(ann->first_layer->last_neuron - 1)->value = 1;
#endif

	last_layer = ann->last_layer;
	for(layer_it = ann->first_layer + 1; layer_it != last_layer; layer_it++)
	{
		last_neuron = layer_it->last_neuron;
		for(neuron_it = layer_it->first_neuron; neuron_it != last_neuron; neuron_it++)
		{
			if(neuron_it->first_con == neuron_it->last_con)
			{
				/* bias neurons */
#ifdef FIXEDFANN
				neuron_it->value = multiplier;
#else
				neuron_it->value = 1;
#endif
				continue;
			}

			activation_function = neuron_it->activation_function;
			steepness = neuron_it->activation_steepness;

			neuron_sum = 0;
			num_connections = neuron_it->last_con - neuron_it->first_con;
			weights = ann->weights + neuron_it->first_con;

			if(ann->connection_rate >= 1)
			{
				if(ann->network_type == FANN_NETTYPE_SHORTCUT)
				{
					neurons = ann->first_layer->first_neuron;
				}
				else
				{
					neurons = (layer_it - 1)->first_neuron;
				}


				/* unrolled loop start */
				i = num_connections & 3;	/* same as modulo 4 */
				switch (i)
				{
					case 3:
						neuron_sum += fann_mult(weights[2], neurons[2].value);
					case 2:
						neuron_sum += fann_mult(weights[1], neurons[1].value);
					case 1:
						neuron_sum += fann_mult(weights[0], neurons[0].value);
					case 0:
						break;
				}

				for(; i != num_connections; i += 4)
				{
					neuron_sum +=
						fann_mult(weights[i], neurons[i].value) +
						fann_mult(weights[i + 1], neurons[i + 1].value) +
						fann_mult(weights[i + 2], neurons[i + 2].value) +
						fann_mult(weights[i + 3], neurons[i + 3].value);
				}
				/* unrolled loop end */

				/*
				 * for(i = 0;i != num_connections; i++){
				 * printf("%f += %f*%f, ", neuron_sum, weights[i], neurons[i].value);
				 * neuron_sum += fann_mult(weights[i], neurons[i].value);
				 * }
				 */
			}
			else
			{
				neuron_pointers = ann->connections + neuron_it->first_con;

				i = num_connections & 3;	/* same as modulo 4 */
				switch (i)
				{
					case 3:
						neuron_sum += fann_mult(weights[2], neuron_pointers[2]->value);
					case 2:
						neuron_sum += fann_mult(weights[1], neuron_pointers[1]->value);
					case 1:
						neuron_sum += fann_mult(weights[0], neuron_pointers[0]->value);
					case 0:
						break;
				}

				for(; i != num_connections; i += 4)
				{
					neuron_sum +=
						fann_mult(weights[i], neuron_pointers[i]->value) +
						fann_mult(weights[i + 1], neuron_pointers[i + 1]->value) +
						fann_mult(weights[i + 2], neuron_pointers[i + 2]->value) +
						fann_mult(weights[i + 3], neuron_pointers[i + 3]->value);
				}
			}

#ifdef FIXEDFANN
			neuron_it->sum = fann_mult(steepness, neuron_sum);

			if(activation_function != last_activation_function || steepness != last_steepness)
			{
				switch (activation_function)
				{
					case FANN_SIGMOID:
					case FANN_SIGMOID_STEPWISE:
						r1 = ann->sigmoid_results[0];
						r2 = ann->sigmoid_results[1];
						r3 = ann->sigmoid_results[2];
						r4 = ann->sigmoid_results[3];
						r5 = ann->sigmoid_results[4];
						r6 = ann->sigmoid_results[5];
						v1 = ann->sigmoid_values[0] / steepness;
						v2 = ann->sigmoid_values[1] / steepness;
						v3 = ann->sigmoid_values[2] / steepness;
						v4 = ann->sigmoid_values[3] / steepness;
						v5 = ann->sigmoid_values[4] / steepness;
						v6 = ann->sigmoid_values[5] / steepness;
						break;
					case FANN_SIGMOID_SYMMETRIC:
					case FANN_SIGMOID_SYMMETRIC_STEPWISE:
						r1 = ann->sigmoid_symmetric_results[0];
						r2 = ann->sigmoid_symmetric_results[1];
						r3 = ann->sigmoid_symmetric_results[2];
						r4 = ann->sigmoid_symmetric_results[3];
						r5 = ann->sigmoid_symmetric_results[4];
						r6 = ann->sigmoid_symmetric_results[5];
						v1 = ann->sigmoid_symmetric_values[0] / steepness;
						v2 = ann->sigmoid_symmetric_values[1] / steepness;
						v3 = ann->sigmoid_symmetric_values[2] / steepness;
						v4 = ann->sigmoid_symmetric_values[3] / steepness;
						v5 = ann->sigmoid_symmetric_values[4] / steepness;
						v6 = ann->sigmoid_symmetric_values[5] / steepness;
						break;
					case FANN_THRESHOLD:
						break;
				}
			}

			switch (activation_function)
			{
				case FANN_SIGMOID:
				case FANN_SIGMOID_STEPWISE:
					neuron_it->value =
						(fann_type) fann_stepwise(v1, v2, v3, v4, v5, v6, r1, r2, r3, r4, r5, r6, 0,
												  multiplier, neuron_sum);
					break;
				case FANN_SIGMOID_SYMMETRIC:
				case FANN_SIGMOID_SYMMETRIC_STEPWISE:
					neuron_it->value =
						(fann_type) fann_stepwise(v1, v2, v3, v4, v5, v6, r1, r2, r3, r4, r5, r6,
												  -multiplier, multiplier, neuron_sum);
					break;
				case FANN_THRESHOLD:
					neuron_it->value = (fann_type) ((neuron_sum < 0) ? 0 : multiplier);
					break;
				case FANN_THRESHOLD_SYMMETRIC:
					neuron_it->value = (fann_type) ((neuron_sum < 0) ? -multiplier : multiplier);
					break;
				case FANN_LINEAR:
					neuron_it->value = neuron_sum;
					break;
				case FANN_LINEAR_PIECE:
					neuron_it->value = (fann_type)((neuron_sum < 0) ? 0 : (neuron_sum > multiplier) ? multiplier : neuron_sum);
					break;
				case FANN_LINEAR_PIECE_SYMMETRIC:
					neuron_it->value = (fann_type)((neuron_sum < -multiplier) ? -multiplier : (neuron_sum > multiplier) ? multiplier : neuron_sum);
					break;
				case FANN_ELLIOT:
				case FANN_ELLIOT_SYMMETRIC:
				case FANN_GAUSSIAN:
				case FANN_GAUSSIAN_SYMMETRIC:
				case FANN_GAUSSIAN_STEPWISE:
				case FANN_SIN_SYMMETRIC:
				case FANN_COS_SYMMETRIC:
					fann_error((struct fann_error *) ann, FANN_E_CANT_USE_ACTIVATION);
					break;
			}
			last_steepness = steepness;
			last_activation_function = activation_function;
#else
			neuron_sum = fann_mult(steepness, neuron_sum);

			max_sum = 150/steepness;
			if(neuron_sum > max_sum)
				neuron_sum = max_sum;
			else if(neuron_sum < -max_sum)
				neuron_sum = -max_sum;

			neuron_it->sum = neuron_sum;

			fann_activation_switch(activation_function, neuron_sum, neuron_it->value);
#endif
		}
	}

	/* set the output */
	output = ann->output;
	num_output = ann->num_output;
	neurons = (ann->last_layer - 1)->first_neuron;
	for(i = 0; i != num_output; i++)
	{
		output[i] = neurons[i].value;
	}
	return ann->output;
}

FANN_EXTERNAL void FANN_API fann_destroy(struct fann *ann)
{
	if(ann == NULL)
		return;
	fann_safe_free(ann->weights);
	fann_safe_free(ann->connections);
	fann_safe_free(ann->first_layer->first_neuron);
	fann_safe_free(ann->first_layer);
	fann_safe_free(ann->output);
	fann_safe_free(ann->train_errors);
	fann_safe_free(ann->train_slopes);
	fann_safe_free(ann->prev_train_slopes);
	fann_safe_free(ann->prev_steps);
	fann_safe_free(ann->prev_weights_deltas);
	fann_safe_free(ann->errstr);
	fann_safe_free(ann->cascade_activation_functions);
	fann_safe_free(ann->cascade_activation_steepnesses);
	fann_safe_free(ann->cascade_candidate_scores);

#ifndef FIXEDFANN
	fann_safe_free( ann->scale_mean_in );
	fann_safe_free( ann->scale_deviation_in );
	fann_safe_free( ann->scale_new_min_in );
	fann_safe_free( ann->scale_factor_in );

	fann_safe_free( ann->scale_mean_out );
	fann_safe_free( ann->scale_deviation_out );
	fann_safe_free( ann->scale_new_min_out );
	fann_safe_free( ann->scale_factor_out );
#endif

	fann_safe_free(ann);
}

FANN_EXTERNAL void FANN_API fann_randomize_weights(struct fann *ann, fann_type min_weight,
												   fann_type max_weight)
{
	fann_type *last_weight;
	fann_type *weights = ann->weights;

	last_weight = weights + ann->total_connections;
	for(; weights != last_weight; weights++)
	{
		*weights = (fann_type) (fann_rand(min_weight, max_weight));
	}

#ifndef FIXEDFANN
	if(ann->prev_train_slopes != NULL)
	{
		fann_clear_train_arrays(ann);
	}
#endif
}

/* deep copy of the fann structure */
FANN_EXTERNAL struct fann* FANN_API fann_copy(struct fann* orig)
{
    struct fann* copy;
    unsigned int num_layers = (unsigned int)(orig->last_layer - orig->first_layer);
    struct fann_layer *orig_layer_it, *copy_layer_it;
    unsigned int layer_size;
    struct fann_neuron *last_neuron,*orig_neuron_it,*copy_neuron_it;
    unsigned int i;
    struct fann_neuron *orig_first_neuron,*copy_first_neuron;
    unsigned int input_neuron;

    copy = fann_allocate_structure(num_layers);
    if (copy==NULL) {
        fann_error((struct fann_error*)orig, FANN_E_CANT_ALLOCATE_MEM);
        return NULL;
    }
    copy->errno_f = orig->errno_f;
    if (orig->errstr)
    {
        copy->errstr = (char *) malloc(FANN_ERRSTR_MAX);
        if (copy->errstr == NULL)
        {
            fann_destroy(copy);
            return NULL;
        }
        strcpy(copy->errstr,orig->errstr);
    }
    copy->error_log = orig->error_log;

    copy->learning_rate = orig->learning_rate;
    copy->learning_momentum = orig->learning_momentum;
    copy->connection_rate = orig->connection_rate;
    copy->network_type = orig->network_type;
    copy->num_MSE = orig->num_MSE;
    copy->MSE_value = orig->MSE_value;
    copy->num_bit_fail = orig->num_bit_fail;
    copy->bit_fail_limit = orig->bit_fail_limit;
    copy->train_error_function = orig->train_error_function;
    copy->train_stop_function = orig->train_stop_function;
	copy->training_algorithm = orig->training_algorithm;
    copy->callback = orig->callback;
	copy->user_data = orig->user_data;
#ifndef FIXEDFANN
    copy->cascade_output_change_fraction = orig->cascade_output_change_fraction;
    copy->cascade_output_stagnation_epochs = orig->cascade_output_stagnation_epochs;
    copy->cascade_candidate_change_fraction = orig->cascade_candidate_change_fraction;
    copy->cascade_candidate_stagnation_epochs = orig->cascade_candidate_stagnation_epochs;
    copy->cascade_best_candidate = orig->cascade_best_candidate;
    copy->cascade_candidate_limit = orig->cascade_candidate_limit;
    copy->cascade_weight_multiplier = orig->cascade_weight_multiplier;
    copy->cascade_max_out_epochs = orig->cascade_max_out_epochs;
    copy->cascade_max_cand_epochs = orig->cascade_max_cand_epochs;

   /* copy cascade activation functions */
    copy->cascade_activation_functions_count = orig->cascade_activation_functions_count;
    copy->cascade_activation_functions = (enum fann_activationfunc_enum *)realloc(copy->cascade_activation_functions,
        copy->cascade_activation_functions_count * sizeof(enum fann_activationfunc_enum));
    if(copy->cascade_activation_functions == NULL)
    {
        fann_error((struct fann_error*)orig, FANN_E_CANT_ALLOCATE_MEM);
        fann_destroy(copy);
        return NULL;
    }
    memcpy(copy->cascade_activation_functions,orig->cascade_activation_functions,
            copy->cascade_activation_functions_count * sizeof(enum fann_activationfunc_enum));

    /* copy cascade activation steepnesses */
    copy->cascade_activation_steepnesses_count = orig->cascade_activation_steepnesses_count;
    copy->cascade_activation_steepnesses = (fann_type *)realloc(copy->cascade_activation_steepnesses, copy->cascade_activation_steepnesses_count * sizeof(fann_type));
    if(copy->cascade_activation_steepnesses == NULL)
    {
        fann_error((struct fann_error*)orig, FANN_E_CANT_ALLOCATE_MEM);
        fann_destroy(copy);
        return NULL;
    }
    memcpy(copy->cascade_activation_steepnesses,orig->cascade_activation_steepnesses,copy->cascade_activation_steepnesses_count * sizeof(fann_type));

    copy->cascade_num_candidate_groups = orig->cascade_num_candidate_groups;

    /* copy candidate scores, if used */
    if (orig->cascade_candidate_scores == NULL)
    {
        copy->cascade_candidate_scores = NULL;
    }
    else
    {
        copy->cascade_candidate_scores =
            (fann_type *) malloc(fann_get_cascade_num_candidates(copy) * sizeof(fann_type));
        if(copy->cascade_candidate_scores == NULL)
        {
            fann_error((struct fann_error *) orig, FANN_E_CANT_ALLOCATE_MEM);
            fann_destroy(copy);
            return NULL;
        }
        memcpy(copy->cascade_candidate_scores,orig->cascade_candidate_scores,fann_get_cascade_num_candidates(copy) * sizeof(fann_type));
    }
#endif /* FIXEDFANN */

    copy->quickprop_decay = orig->quickprop_decay;
    copy->quickprop_mu = orig->quickprop_mu;
    copy->rprop_increase_factor = orig->rprop_increase_factor;
    copy->rprop_decrease_factor = orig->rprop_decrease_factor;
    copy->rprop_delta_min = orig->rprop_delta_min;
    copy->rprop_delta_max = orig->rprop_delta_max;
    copy->rprop_delta_zero = orig->rprop_delta_zero;

    /* user_data is not deep copied.  user should use fann_copy_with_user_data() for that */
    copy->user_data = orig->user_data;

#ifdef FIXEDFANN
    copy->decimal_point = orig->decimal_point;
    copy->multiplier = orig->multiplier;
    memcpy(copy->sigmoid_results,orig->sigmoid_results,6*sizeof(fann_type));
    memcpy(copy->sigmoid_values,orig->sigmoid_values,6*sizeof(fann_type));
    memcpy(copy->sigmoid_symmetric_results,orig->sigmoid_symmetric_results,6*sizeof(fann_type));
    memcpy(copy->sigmoid_symmetric_values,orig->sigmoid_symmetric_values,6*sizeof(fann_type));
#endif


    /* copy layer sizes, prepare for fann_allocate_neurons */
    for (orig_layer_it = orig->first_layer, copy_layer_it = copy->first_layer;
            orig_layer_it != orig->last_layer; orig_layer_it++, copy_layer_it++)
    {
        layer_size = (unsigned int)(orig_layer_it->last_neuron - orig_layer_it->first_neuron);
        copy_layer_it->first_neuron = NULL;
        copy_layer_it->last_neuron = copy_layer_it->first_neuron + layer_size;
        copy->total_neurons += layer_size;
    }
    copy->num_input = orig->num_input;
    copy->num_output = orig->num_output;


    /* copy scale parameters, when used */
#ifndef FIXEDFANN
    if (orig->scale_mean_in != NULL)
    {
        fann_allocate_scale(copy);
        for (i=0; i < orig->num_input ; i++) {
            copy->scale_mean_in[i] = orig->scale_mean_in[i];
            copy->scale_deviation_in[i] = orig->scale_deviation_in[i];
            copy->scale_new_min_in[i] = orig->scale_new_min_in[i];
            copy->scale_factor_in[i] = orig->scale_factor_in[i];
        }
        for (i=0; i < orig->num_output ; i++) {
            copy->scale_mean_out[i] = orig->scale_mean_out[i];
            copy->scale_deviation_out[i] = orig->scale_deviation_out[i];
            copy->scale_new_min_out[i] = orig->scale_new_min_out[i];
            copy->scale_factor_out[i] = orig->scale_factor_out[i];
        }
    }
#endif

    /* copy the neurons */
    fann_allocate_neurons(copy);
    if (copy->errno_f == FANN_E_CANT_ALLOCATE_MEM)
    {
        fann_destroy(copy);
        return NULL;
    }
    layer_size = (unsigned int)((orig->last_layer-1)->last_neuron - (orig->last_layer-1)->first_neuron);
    memcpy(copy->output,orig->output, layer_size * sizeof(fann_type));

    last_neuron = (orig->last_layer - 1)->last_neuron;
    for (orig_neuron_it = orig->first_layer->first_neuron, copy_neuron_it = copy->first_layer->first_neuron;
            orig_neuron_it != last_neuron; orig_neuron_it++, copy_neuron_it++)
    {
        memcpy(copy_neuron_it,orig_neuron_it,sizeof(struct fann_neuron));
    }
 /* copy the connections */
    copy->total_connections = orig->total_connections;
    fann_allocate_connections(copy);
    if (copy->errno_f == FANN_E_CANT_ALLOCATE_MEM)
    {
        fann_destroy(copy);
        return NULL;
    }

    orig_first_neuron = orig->first_layer->first_neuron;
    copy_first_neuron = copy->first_layer->first_neuron;
    for (i=0; i < orig->total_connections; i++)
    {
        copy->weights[i] = orig->weights[i];
        input_neuron = (unsigned int)(orig->connections[i] - orig_first_neuron);
        copy->connections[i] = copy_first_neuron + input_neuron;
    }

    if (orig->train_slopes)
    {
        copy->train_slopes = (fann_type *) malloc(copy->total_connections_allocated * sizeof(fann_type));
        if (copy->train_slopes == NULL)
        {
            fann_error((struct fann_error *) orig, FANN_E_CANT_ALLOCATE_MEM);
            fann_destroy(copy);
            return NULL;
        }
        memcpy(copy->train_slopes,orig->train_slopes,copy->total_connections_allocated * sizeof(fann_type));
    }

    if (orig->prev_steps)
    {
        copy->prev_steps = (fann_type *) malloc(copy->total_connections_allocated * sizeof(fann_type));
        if (copy->prev_steps == NULL)
        {
            fann_error((struct fann_error *) orig, FANN_E_CANT_ALLOCATE_MEM);
            fann_destroy(copy);
            return NULL;
        }
        memcpy(copy->prev_steps, orig->prev_steps, copy->total_connections_allocated * sizeof(fann_type));
    }

    if (orig->prev_train_slopes)
    {
        copy->prev_train_slopes = (fann_type *) malloc(copy->total_connections_allocated * sizeof(fann_type));
        if (copy->prev_train_slopes == NULL)
        {
            fann_error((struct fann_error *) orig, FANN_E_CANT_ALLOCATE_MEM);
            fann_destroy(copy);
            return NULL;
        }
        memcpy(copy->prev_train_slopes,orig->prev_train_slopes, copy->total_connections_allocated * sizeof(fann_type));
    }

    if (orig->prev_weights_deltas)
    {
        copy->prev_weights_deltas = (fann_type *) malloc(copy->total_connections_allocated * sizeof(fann_type));
        if(copy->prev_weights_deltas == NULL)
        {
            fann_error((struct fann_error *) orig, FANN_E_CANT_ALLOCATE_MEM);
            fann_destroy(copy);
            return NULL;
        }
        memcpy(copy->prev_weights_deltas, orig->prev_weights_deltas,copy->total_connections_allocated * sizeof(fann_type));
    }

    return copy;
}

FANN_EXTERNAL void FANN_API fann_print_connections(struct fann *ann)
{
	struct fann_layer *layer_it;
	struct fann_neuron *neuron_it;
	unsigned int i;
	int value;
	char *neurons;
	unsigned int num_neurons = fann_get_total_neurons(ann) - fann_get_num_output(ann);

	neurons = (char *) malloc(num_neurons + 1);
	if(neurons == NULL)
	{
		fann_error(NULL, FANN_E_CANT_ALLOCATE_MEM);
		return;
	}
	neurons[num_neurons] = 0;

	printf("Layer / Neuron ");
	for(i = 0; i < num_neurons; i++)
	{
		printf("%d", i % 10);
	}
	printf("\n");

	for(layer_it = ann->first_layer + 1; layer_it != ann->last_layer; layer_it++)
	{
		for(neuron_it = layer_it->first_neuron; neuron_it != layer_it->last_neuron; neuron_it++)
		{

			memset(neurons, (int) '.', num_neurons);
			for(i = neuron_it->first_con; i < neuron_it->last_con; i++)
			{
				if(ann->weights[i] < 0)
				{
#ifdef FIXEDFANN
					value = (int) ((ann->weights[i] / (double) ann->multiplier) - 0.5);
#else
					value = (int) ((ann->weights[i]) - 0.5);
#endif
					if(value < -25)
						value = -25;
					neurons[ann->connections[i] - ann->first_layer->first_neuron] = (char)('a' - value);
				}
				else
				{
#ifdef FIXEDFANN
					value = (int) ((ann->weights[i] / (double) ann->multiplier) + 0.5);
#else
					value = (int) ((ann->weights[i]) + 0.5);
#endif
					if(value > 25)
						value = 25;
					neurons[ann->connections[i] - ann->first_layer->first_neuron] = (char)('A' + value);
				}
			}
			printf("L %3d / N %4d %s\n", (int)(layer_it - ann->first_layer),
				   (int)(neuron_it - ann->first_layer->first_neuron), neurons);
		}
	}

	free(neurons);
}

/* Initialize the weights using Widrow + Nguyen's algorithm.
*/
FANN_EXTERNAL void FANN_API fann_init_weights(struct fann *ann, struct fann_train_data *train_data)
{
	fann_type smallest_inp, largest_inp;
	unsigned int dat = 0, elem, num_connect, num_hidden_neurons;
	struct fann_layer *layer_it;
	struct fann_neuron *neuron_it, *last_neuron, *bias_neuron;

#ifdef FIXEDFANN
	unsigned int multiplier = ann->multiplier;
#endif
	float scale_factor;

	for(smallest_inp = largest_inp = train_data->input[0][0]; dat < train_data->num_data; dat++)
	{
		for(elem = 0; elem < train_data->num_input; elem++)
		{
			if(train_data->input[dat][elem] < smallest_inp)
				smallest_inp = train_data->input[dat][elem];
			if(train_data->input[dat][elem] > largest_inp)
				largest_inp = train_data->input[dat][elem];
		}
	}

	num_hidden_neurons = (unsigned int)(
		ann->total_neurons - (ann->num_input + ann->num_output +
							  (ann->last_layer - ann->first_layer)));
	scale_factor =
		(float) (pow
				 ((double) (0.7f * (double) num_hidden_neurons),
				  (double) (1.0f / (double) ann->num_input)) / (double) (largest_inp -
																		 smallest_inp));

#ifdef DEBUG
	printf("Initializing weights with scale factor %f\n", scale_factor);
#endif
	bias_neuron = ann->first_layer->last_neuron - 1;
	for(layer_it = ann->first_layer + 1; layer_it != ann->last_layer; layer_it++)
	{
		last_neuron = layer_it->last_neuron;

		if(ann->network_type == FANN_NETTYPE_LAYER)
		{
			bias_neuron = (layer_it - 1)->last_neuron - 1;
		}

		for(neuron_it = layer_it->first_neuron; neuron_it != last_neuron; neuron_it++)
		{
			for(num_connect = neuron_it->first_con; num_connect < neuron_it->last_con;
				num_connect++)
			{
				if(bias_neuron == ann->connections[num_connect])
				{
#ifdef FIXEDFANN
					ann->weights[num_connect] =
						(fann_type) fann_rand(-scale_factor, scale_factor * multiplier);
#else
					ann->weights[num_connect] = (fann_type) fann_rand(-scale_factor, scale_factor);
#endif
				}
				else
				{
#ifdef FIXEDFANN
					ann->weights[num_connect] = (fann_type) fann_rand(0, scale_factor * multiplier);
#else
					ann->weights[num_connect] = (fann_type) fann_rand(0, scale_factor);
#endif
				}
			}
		}
	}

#ifndef FIXEDFANN
	if(ann->prev_train_slopes != NULL)
	{
		fann_clear_train_arrays(ann);
	}
#endif
}

FANN_EXTERNAL void FANN_API fann_print_parameters(struct fann *ann)
{
	struct fann_layer *layer_it;
#ifndef FIXEDFANN
	unsigned int i;
#endif

	printf("Input layer                          :%4d neurons, 1 bias\n", ann->num_input);
	for(layer_it = ann->first_layer + 1; layer_it != ann->last_layer - 1; layer_it++)
	{
		if(ann->network_type == FANN_NETTYPE_SHORTCUT)
		{
			printf("  Hidden layer                       :%4d neurons, 0 bias\n",
				   (int)(layer_it->last_neuron - layer_it->first_neuron));
		}
		else
		{
			printf("  Hidden layer                       :%4d neurons, 1 bias\n",
				   (int)(layer_it->last_neuron - layer_it->first_neuron - 1));
		}
	}
	printf("Output layer                         :%4d neurons\n", ann->num_output);
	printf("Total neurons and biases             :%4d\n", fann_get_total_neurons(ann));
	printf("Total connections                    :%4d\n", ann->total_connections);
	printf("Connection rate                      :%8.3f\n", ann->connection_rate);
	printf("Network type                         :   %s\n", FANN_NETTYPE_NAMES[ann->network_type]);
#ifdef FIXEDFANN
	printf("Decimal point                        :%4d\n", ann->decimal_point);
	printf("Multiplier                           :%4d\n", ann->multiplier);
#else
	printf("Training algorithm                   :   %s\n", FANN_TRAIN_NAMES[ann->training_algorithm]);
	printf("Training error function              :   %s\n", FANN_ERRORFUNC_NAMES[ann->train_error_function]);
	printf("Training stop function               :   %s\n", FANN_STOPFUNC_NAMES[ann->train_stop_function]);
#endif
#ifdef FIXEDFANN
	printf("Bit fail limit                       :%4d\n", ann->bit_fail_limit);
#else
	printf("Bit fail limit                       :%8.3f\n", ann->bit_fail_limit);
	printf("Learning rate                        :%8.3f\n", ann->learning_rate);
	printf("Learning momentum                    :%8.3f\n", ann->learning_momentum);
	printf("Quickprop decay                      :%11.6f\n", ann->quickprop_decay);
	printf("Quickprop mu                         :%8.3f\n", ann->quickprop_mu);
	printf("RPROP increase factor                :%8.3f\n", ann->rprop_increase_factor);
	printf("RPROP decrease factor                :%8.3f\n", ann->rprop_decrease_factor);
	printf("RPROP delta min                      :%8.3f\n", ann->rprop_delta_min);
	printf("RPROP delta max                      :%8.3f\n", ann->rprop_delta_max);
	printf("Cascade output change fraction       :%11.6f\n", ann->cascade_output_change_fraction);
	printf("Cascade candidate change fraction    :%11.6f\n", ann->cascade_candidate_change_fraction);
	printf("Cascade output stagnation epochs     :%4d\n", ann->cascade_output_stagnation_epochs);
	printf("Cascade candidate stagnation epochs  :%4d\n", ann->cascade_candidate_stagnation_epochs);
	printf("Cascade max output epochs            :%4d\n", ann->cascade_max_out_epochs);
	printf("Cascade min output epochs            :%4d\n", ann->cascade_min_out_epochs);
	printf("Cascade max candidate epochs         :%4d\n", ann->cascade_max_cand_epochs);
	printf("Cascade min candidate epochs         :%4d\n", ann->cascade_min_cand_epochs);
	printf("Cascade weight multiplier            :%8.3f\n", ann->cascade_weight_multiplier);
	printf("Cascade candidate limit              :%8.3f\n", ann->cascade_candidate_limit);
	for(i = 0; i < ann->cascade_activation_functions_count; i++)
		printf("Cascade activation functions[%d]      :   %s\n", i,
			FANN_ACTIVATIONFUNC_NAMES[ann->cascade_activation_functions[i]]);
	for(i = 0; i < ann->cascade_activation_steepnesses_count; i++)
		printf("Cascade activation steepnesses[%d]    :%8.3f\n", i,
			ann->cascade_activation_steepnesses[i]);

	printf("Cascade candidate groups             :%4d\n", ann->cascade_num_candidate_groups);
	printf("Cascade no. of candidates            :%4d\n", fann_get_cascade_num_candidates(ann));

	/* TODO: dump scale parameters */
#endif
}

FANN_GET(unsigned int, num_input)
FANN_GET(unsigned int, num_output)

FANN_EXTERNAL unsigned int FANN_API fann_get_total_neurons(struct fann *ann)
{
	if(ann->network_type)
	{
		return ann->total_neurons;
	}
	else
	{
		/* -1, because there is always an unused bias neuron in the last layer */
		return ann->total_neurons - 1;
	}
}

FANN_GET(unsigned int, total_connections)

FANN_EXTERNAL enum fann_nettype_enum FANN_API fann_get_network_type(struct fann *ann)
{
    /* Currently two types: LAYER = 0, SHORTCUT = 1 */
    /* Enum network_types must be set to match the return values  */
    return ann->network_type;
}

FANN_EXTERNAL float FANN_API fann_get_connection_rate(struct fann *ann)
{
    return ann->connection_rate;
}

FANN_EXTERNAL unsigned int FANN_API fann_get_num_layers(struct fann *ann)
{
    return (unsigned int)(ann->last_layer - ann->first_layer);
}

FANN_EXTERNAL void FANN_API fann_get_layer_array(struct fann *ann, unsigned int *layers)
{
    struct fann_layer *layer_it;

    for (layer_it = ann->first_layer; layer_it != ann->last_layer; layer_it++) {
        unsigned int count = (unsigned int)(layer_it->last_neuron - layer_it->first_neuron);
        /* Remove the bias from the count of neurons. */
        switch (fann_get_network_type(ann)) {
            case FANN_NETTYPE_LAYER: {
                --count;
                break;
            }
            case FANN_NETTYPE_SHORTCUT: {
                /* The bias in the first layer is reused for all layers */
                if (layer_it == ann->first_layer)
                    --count;
                break;
            }
            default: {
                /* Unknown network type, assume no bias present  */
                break;
            }
        }
        *layers++ = count;
    }
}

FANN_EXTERNAL void FANN_API fann_get_bias_array(struct fann *ann, unsigned int *bias)
{
    struct fann_layer *layer_it;

    for (layer_it = ann->first_layer; layer_it != ann->last_layer; ++layer_it, ++bias) {
        switch (fann_get_network_type(ann)) {
            case FANN_NETTYPE_LAYER: {
                /* Report one bias in each layer except the last */
                if (layer_it != ann->last_layer-1)
                    *bias = 1;
                else
                    *bias = 0;
                break;
            }
            case FANN_NETTYPE_SHORTCUT: {
                /* The bias in the first layer is reused for all layers */
                if (layer_it == ann->first_layer)
                    *bias = 1;
                else
                    *bias = 0;
                break;
            }
            default: {
                /* Unknown network type, assume no bias present  */
                *bias = 0;
                break;
            }
        }
    }
}

FANN_EXTERNAL void FANN_API fann_get_connection_array(struct fann *ann, struct fann_connection *connections)
{
    struct fann_neuron *first_neuron;
    struct fann_layer *layer_it;
    struct fann_neuron *neuron_it;
    unsigned int idx;
    unsigned int source_index;
    unsigned int destination_index;

    first_neuron = ann->first_layer->first_neuron;

    source_index = 0;
    destination_index = 0;

    /* The following assumes that the last unused bias has no connections */

    /* for each layer */
    for(layer_it = ann->first_layer; layer_it != ann->last_layer; layer_it++){
        /* for each neuron */
        for(neuron_it = layer_it->first_neuron; neuron_it != layer_it->last_neuron; neuron_it++){
            /* for each connection */
            for (idx = neuron_it->first_con; idx < neuron_it->last_con; idx++){
                /* Assign the source, destination and weight */
                connections->from_neuron = (unsigned int)(ann->connections[source_index] - first_neuron);
                connections->to_neuron = destination_index;
                connections->weight = ann->weights[source_index];

                connections++;
                source_index++;
            }
            destination_index++;
        }
    }
}

FANN_EXTERNAL void FANN_API fann_set_weight_array(struct fann *ann,
    struct fann_connection *connections, unsigned int num_connections)
{
    unsigned int idx;

    for (idx = 0; idx < num_connections; idx++) {
        fann_set_weight(ann, connections[idx].from_neuron,
            connections[idx].to_neuron, connections[idx].weight);
    }
}

FANN_EXTERNAL void FANN_API fann_set_weight(struct fann *ann,
    unsigned int from_neuron, unsigned int to_neuron, fann_type weight)
{
    struct fann_neuron *first_neuron;
    struct fann_layer *layer_it;
    struct fann_neuron *neuron_it;
    unsigned int idx;
    unsigned int source_index;
    unsigned int destination_index;

    first_neuron = ann->first_layer->first_neuron;

    source_index = 0;
    destination_index = 0;

    /* Find the connection, simple brute force search through the network
       for one or more connections that match to minimize datastructure dependencies.
       Nothing is done if the connection does not already exist in the network. */

    /* for each layer */
    for(layer_it = ann->first_layer; layer_it != ann->last_layer; layer_it++){
        /* for each neuron */
        for(neuron_it = layer_it->first_neuron; neuron_it != layer_it->last_neuron; neuron_it++){
            /* for each connection */
            for (idx = neuron_it->first_con; idx < neuron_it->last_con; idx++){
                /* If the source and destination neurons match, assign the weight */
                if (((int)from_neuron == ann->connections[source_index] - first_neuron) &&
                    (to_neuron == destination_index))
                {
                    ann->weights[source_index] = weight;
                }
                source_index++;
            }
            destination_index++;
        }
    }
}

FANN_EXTERNAL void FANN_API fann_get_weights(struct fann *ann, fann_type *weights)
{
	memcpy(weights, ann->weights, sizeof(fann_type)*ann->total_connections);
}

FANN_EXTERNAL void FANN_API fann_set_weights(struct fann *ann, fann_type *weights)
{
	memcpy(ann->weights, weights, sizeof(fann_type)*ann->total_connections);
}

FANN_GET_SET(void *, user_data)

#ifdef FIXEDFANN

FANN_GET(unsigned int, decimal_point)
FANN_GET(unsigned int, multiplier)

/* INTERNAL FUNCTION
   Adjust the steepwise functions (if used)
*/
void fann_update_stepwise(struct fann *ann)
{
	unsigned int i = 0;

	/* Calculate the parameters for the stepwise linear
	 * sigmoid function fixed point.
	 * Using a rewritten sigmoid function.
	 * results 0.005, 0.05, 0.25, 0.75, 0.95, 0.995
	 */
	ann->sigmoid_results[0] = fann_max((fann_type) (ann->multiplier / 200.0 + 0.5), 1);
	ann->sigmoid_results[1] = fann_max((fann_type) (ann->multiplier / 20.0 + 0.5), 1);
	ann->sigmoid_results[2] = fann_max((fann_type) (ann->multiplier / 4.0 + 0.5), 1);
	ann->sigmoid_results[3] = fann_min(ann->multiplier - (fann_type) (ann->multiplier / 4.0 + 0.5), ann->multiplier - 1);
	ann->sigmoid_results[4] = fann_min(ann->multiplier - (fann_type) (ann->multiplier / 20.0 + 0.5), ann->multiplier - 1);
	ann->sigmoid_results[5] = fann_min(ann->multiplier - (fann_type) (ann->multiplier / 200.0 + 0.5), ann->multiplier - 1);

	ann->sigmoid_symmetric_results[0] = fann_max((fann_type) ((ann->multiplier / 100.0) - ann->multiplier - 0.5),
				                                 (fann_type) (1 - (fann_type) ann->multiplier));
	ann->sigmoid_symmetric_results[1] =	fann_max((fann_type) ((ann->multiplier / 10.0) - ann->multiplier - 0.5),
				                                 (fann_type) (1 - (fann_type) ann->multiplier));
	ann->sigmoid_symmetric_results[2] =	fann_max((fann_type) ((ann->multiplier / 2.0) - ann->multiplier - 0.5),
                                				 (fann_type) (1 - (fann_type) ann->multiplier));
	ann->sigmoid_symmetric_results[3] = fann_min(ann->multiplier - (fann_type) (ann->multiplier / 2.0 + 0.5),
				 							     ann->multiplier - 1);
	ann->sigmoid_symmetric_results[4] = fann_min(ann->multiplier - (fann_type) (ann->multiplier / 10.0 + 0.5),
				 							     ann->multiplier - 1);
	ann->sigmoid_symmetric_results[5] = fann_min(ann->multiplier - (fann_type) (ann->multiplier / 100.0 + 1.0),
				 							     ann->multiplier - 1);

	for(i = 0; i < 6; i++)
	{
		ann->sigmoid_values[i] =
			(fann_type) (((log(ann->multiplier / (float) ann->sigmoid_results[i] - 1) *
						   (float) ann->multiplier) / -2.0) * (float) ann->multiplier);
		ann->sigmoid_symmetric_values[i] =
			(fann_type) (((log
						   ((ann->multiplier -
							 (float) ann->sigmoid_symmetric_results[i]) /
							((float) ann->sigmoid_symmetric_results[i] +
							 ann->multiplier)) * (float) ann->multiplier) / -2.0) *
						 (float) ann->multiplier);
	}
}
#endif


/* INTERNAL FUNCTION
   Allocates the main structure and sets some default values.
 */
struct fann *fann_allocate_structure(unsigned int num_layers)
{
	struct fann *ann;

	if(num_layers < 2)
	{
#ifdef DEBUG
		printf("less than 2 layers - ABORTING.\n");
#endif
		return NULL;
	}

	/* allocate and initialize the main network structure */
	ann = (struct fann *) malloc(sizeof(struct fann));
	if(ann == NULL)
	{
		fann_error(NULL, FANN_E_CANT_ALLOCATE_MEM);
		return NULL;
	}

	ann->errno_f = FANN_E_NO_ERROR;
	ann->error_log = fann_default_error_log;
	ann->errstr = NULL;
	ann->learning_rate = 0.7f;
	ann->learning_momentum = 0.0;
	ann->total_neurons = 0;
	ann->total_connections = 0;
	ann->num_input = 0;
	ann->num_output = 0;
	ann->train_errors = NULL;
	ann->train_slopes = NULL;
	ann->prev_steps = NULL;
	ann->prev_train_slopes = NULL;
	ann->prev_weights_deltas = NULL;
	ann->training_algorithm = FANN_TRAIN_RPROP;
	ann->num_MSE = 0;
	ann->MSE_value = 0;
	ann->num_bit_fail = 0;
	ann->bit_fail_limit = (fann_type)0.35;
	ann->network_type = FANN_NETTYPE_LAYER;
	ann->train_error_function = FANN_ERRORFUNC_TANH;
	ann->train_stop_function = FANN_STOPFUNC_MSE;
	ann->callback = NULL;
    ann->user_data = NULL; /* User is responsible for deallocation */
	ann->weights = NULL;
	ann->connections = NULL;
	ann->output = NULL;
#ifndef FIXEDFANN
	ann->scale_mean_in = NULL;
	ann->scale_deviation_in = NULL;
	ann->scale_new_min_in = NULL;
	ann->scale_factor_in = NULL;
	ann->scale_mean_out = NULL;
	ann->scale_deviation_out = NULL;
	ann->scale_new_min_out = NULL;
	ann->scale_factor_out = NULL;
#endif

	/* variables used for cascade correlation (reasonable defaults) */
	ann->cascade_output_change_fraction = 0.01f;
	ann->cascade_candidate_change_fraction = 0.01f;
	ann->cascade_output_stagnation_epochs = 12;
	ann->cascade_candidate_stagnation_epochs = 12;
	ann->cascade_num_candidate_groups = 2;
	ann->cascade_weight_multiplier = (fann_type)0.4;
	ann->cascade_candidate_limit = (fann_type)1000.0;
	ann->cascade_max_out_epochs = 150;
	ann->cascade_max_cand_epochs = 150;
	ann->cascade_min_out_epochs = 50;
	ann->cascade_min_cand_epochs = 50;
	ann->cascade_candidate_scores = NULL;
	ann->cascade_activation_functions_count = 10;
	ann->cascade_activation_functions =
		(enum fann_activationfunc_enum *)calloc(ann->cascade_activation_functions_count,
							   sizeof(enum fann_activationfunc_enum));
	if(ann->cascade_activation_functions == NULL)
	{
		fann_error(NULL, FANN_E_CANT_ALLOCATE_MEM);
		free(ann);
		return NULL;
	}

	ann->cascade_activation_functions[0] = FANN_SIGMOID;
	ann->cascade_activation_functions[1] = FANN_SIGMOID_SYMMETRIC;
	ann->cascade_activation_functions[2] = FANN_GAUSSIAN;
	ann->cascade_activation_functions[3] = FANN_GAUSSIAN_SYMMETRIC;
	ann->cascade_activation_functions[4] = FANN_ELLIOT;
	ann->cascade_activation_functions[5] = FANN_ELLIOT_SYMMETRIC;
	ann->cascade_activation_functions[6] = FANN_SIN_SYMMETRIC;
	ann->cascade_activation_functions[7] = FANN_COS_SYMMETRIC;
	ann->cascade_activation_functions[8] = FANN_SIN;
	ann->cascade_activation_functions[9] = FANN_COS;

	ann->cascade_activation_steepnesses_count = 4;
	ann->cascade_activation_steepnesses =
		(fann_type *)calloc(ann->cascade_activation_steepnesses_count,
							   sizeof(fann_type));
	if(ann->cascade_activation_steepnesses == NULL)
	{
		fann_safe_free(ann->cascade_activation_functions);
		fann_error(NULL, FANN_E_CANT_ALLOCATE_MEM);
		free(ann);
		return NULL;
	}

	ann->cascade_activation_steepnesses[0] = (fann_type)0.25;
	ann->cascade_activation_steepnesses[1] = (fann_type)0.5;
	ann->cascade_activation_steepnesses[2] = (fann_type)0.75;
	ann->cascade_activation_steepnesses[3] = (fann_type)1.0;

	/* Variables for use with with Quickprop training (reasonable defaults) */
	ann->quickprop_decay = -0.0001f;
	ann->quickprop_mu = 1.75;

	/* Variables for use with with RPROP training (reasonable defaults) */
	ann->rprop_increase_factor = 1.2f;
	ann->rprop_decrease_factor = 0.5;
	ann->rprop_delta_min = 0.0;
	ann->rprop_delta_max = 50.0;
	ann->rprop_delta_zero = 0.1f;

 	/* Variables for use with SARPROP training (reasonable defaults) */
 	ann->sarprop_weight_decay_shift = -6.644f;
 	ann->sarprop_step_error_threshold_factor = 0.1f;
 	ann->sarprop_step_error_shift = 1.385f;
 	ann->sarprop_temperature = 0.015f;
 	ann->sarprop_epoch = 0;

	fann_init_error_data((struct fann_error *) ann);

#ifdef FIXEDFANN
	/* these values are only boring defaults, and should really
	 * never be used, since the real values are always loaded from a file. */
	ann->decimal_point = 8;
	ann->multiplier = 256;
#endif

	/* allocate room for the layers */
	ann->first_layer = (struct fann_layer *) calloc(num_layers, sizeof(struct fann_layer));
	if(ann->first_layer == NULL)
	{
		fann_error(NULL, FANN_E_CANT_ALLOCATE_MEM);
		free(ann);
		return NULL;
	}

	ann->last_layer = ann->first_layer + num_layers;

	return ann;
}

/* INTERNAL FUNCTION
   Allocates room for the scaling parameters.
 */
int fann_allocate_scale(struct fann *ann)
{
	/* todo this should only be allocated when needed */
#ifndef FIXEDFANN
	unsigned int i = 0;
#define SCALE_ALLOCATE( what, where, default_value )		    			\
		ann->what##_##where = (float *)calloc(								\
			ann->num_##where##put,											\
			sizeof( float )													\
			);																\
		if( ann->what##_##where == NULL )									\
		{																	\
			fann_error( NULL, FANN_E_CANT_ALLOCATE_MEM );					\
			fann_destroy( ann );                            				\
			return 1;														\
		}																	\
		for( i = 0; i < ann->num_##where##put; i++ )						\
			ann->what##_##where[ i ] = ( default_value );

	SCALE_ALLOCATE( scale_mean,		in,		0.0 )
	SCALE_ALLOCATE( scale_deviation,	in,		1.0 )
	SCALE_ALLOCATE( scale_new_min,	in,		-1.0 )
	SCALE_ALLOCATE( scale_factor,		in,		1.0 )

	SCALE_ALLOCATE( scale_mean,		out,	0.0 )
	SCALE_ALLOCATE( scale_deviation,	out,	1.0 )
	SCALE_ALLOCATE( scale_new_min,	out,	-1.0 )
	SCALE_ALLOCATE( scale_factor,		out,	1.0 )
#undef SCALE_ALLOCATE
#endif
	return 0;
}

/* INTERNAL FUNCTION
   Allocates room for the neurons.
 */
void fann_allocate_neurons(struct fann *ann)
{
	struct fann_layer *layer_it;
	struct fann_neuron *neurons;
	unsigned int num_neurons_so_far = 0;
	unsigned int num_neurons = 0;

	/* all the neurons is allocated in one long array (calloc clears mem) */
	neurons = (struct fann_neuron *) calloc(ann->total_neurons, sizeof(struct fann_neuron));
	ann->total_neurons_allocated = ann->total_neurons;

	if(neurons == NULL)
	{
		fann_error((struct fann_error *) ann, FANN_E_CANT_ALLOCATE_MEM);
		return;
	}

	for(layer_it = ann->first_layer; layer_it != ann->last_layer; layer_it++)
	{
		num_neurons = (unsigned int)(layer_it->last_neuron - layer_it->first_neuron);
		layer_it->first_neuron = neurons + num_neurons_so_far;
		layer_it->last_neuron = layer_it->first_neuron + num_neurons;
		num_neurons_so_far += num_neurons;
	}

	ann->output = (fann_type *) calloc(num_neurons, sizeof(fann_type));
	if(ann->output == NULL)
	{
		fann_error((struct fann_error *) ann, FANN_E_CANT_ALLOCATE_MEM);
		return;
	}
}

/* INTERNAL FUNCTION
   Allocate room for the connections.
 */
void fann_allocate_connections(struct fann *ann)
{
	ann->weights = (fann_type *) calloc(ann->total_connections, sizeof(fann_type));
	if(ann->weights == NULL)
	{
		fann_error((struct fann_error *) ann, FANN_E_CANT_ALLOCATE_MEM);
		return;
	}
	ann->total_connections_allocated = ann->total_connections;

	/* TODO make special cases for all places where the connections
	 * is used, so that it is not needed for fully connected networks.
	 */
	ann->connections =
		(struct fann_neuron **) calloc(ann->total_connections_allocated,
									   sizeof(struct fann_neuron *));
	if(ann->connections == NULL)
	{
		fann_error((struct fann_error *) ann, FANN_E_CANT_ALLOCATE_MEM);
		return;
	}
}

#ifdef FANN_NO_SEED
int FANN_SEED_RAND = 0;
#else
int FANN_SEED_RAND = 1;
#endif

FANN_EXTERNAL void FANN_API fann_disable_seed_rand()
{
    FANN_SEED_RAND = 0;
}

FANN_EXTERNAL void FANN_API fann_enable_seed_rand()
{
    FANN_SEED_RAND = 1;
}

/* INTERNAL FUNCTION
   Seed the random function.
 */
void fann_seed_rand()
{
#ifndef _WIN32
	FILE *fp = fopen("/dev/urandom", "r");
	unsigned int foo;
	struct timeval t;

	if(!fp)
	{
		gettimeofday(&t, NULL);
		foo = t.tv_usec;
#ifdef DEBUG
		printf("unable to open /dev/urandom\n");
#endif
	}
	else
	{
	        if(fread(&foo, sizeof(foo), 1, fp) != 1)
	        {
  		       gettimeofday(&t, NULL);
		       foo = t.tv_usec;
#ifdef DEBUG
		       printf("unable to read from /dev/urandom\n");
#endif
		}
		fclose(fp);
	}
    if(FANN_SEED_RAND) {
        srand(foo);
    }
#else
	/* COMPAT_TIME REPLACEMENT */
    if(FANN_SEED_RAND) {
    	srand(GetTickCount());
    }
#endif
}


/*
file https://github.com/kaizhu256/fann/blob/v2021.2.21/src/fann_io.c
*/
/*
  Fast Artificial Neural Network Library (fann)
  Copyright (C) 2003-2016 Steffen Nissen (steffen.fann@gmail.com)

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <limits.h>

// #include "config.h"
// #include "fann.h"
// #include "fann_data.h"

/* Create a network from a configuration file.
 */
FANN_EXTERNAL struct fann *FANN_API fann_create_from_file(const char *configuration_file)
{
	struct fann *ann;
	FILE *conf = fopen(configuration_file, "r");

	if(!conf)
	{
		fann_error(NULL, FANN_E_CANT_OPEN_CONFIG_R, configuration_file);
		return NULL;
	}
	ann = fann_create_from_fd(conf, configuration_file);
	fclose(conf);
	return ann;
}

/* Save the network.
 */
FANN_EXTERNAL int FANN_API fann_save(struct fann *ann, const char *configuration_file)
{
	return fann_save_internal(ann, configuration_file, 0);
}

/* Save the network as fixed point data.
 */
FANN_EXTERNAL int FANN_API fann_save_to_fixed(struct fann *ann, const char *configuration_file)
{
	return fann_save_internal(ann, configuration_file, 1);
}

/* INTERNAL FUNCTION
   Used to save the network to a file.
 */
int fann_save_internal(struct fann *ann, const char *configuration_file, unsigned int save_as_fixed)
{
	int retval;
	FILE *conf = fopen(configuration_file, "w+");

	if(!conf)
	{
		fann_error((struct fann_error *) ann, FANN_E_CANT_OPEN_CONFIG_W, configuration_file);
		return -1;
	}
	retval = fann_save_internal_fd(ann, conf, configuration_file, save_as_fixed);
	fclose(conf);
	return retval;
}

/* INTERNAL FUNCTION
   Used to save the network to a file descriptor.
 */
int fann_save_internal_fd(struct fann *ann, FILE * conf, const char *configuration_file,
						  unsigned int save_as_fixed)
{
	struct fann_layer *layer_it;
	int calculated_decimal_point = 0;
	struct fann_neuron *neuron_it, *first_neuron;
	fann_type *weights;
	struct fann_neuron **connected_neurons;
	unsigned int i = 0;

#ifndef FIXEDFANN
	/* variabels for use when saving floats as fixed point variabels */
	unsigned int decimal_point = 0;
	unsigned int fixed_multiplier = 0;
	fann_type max_possible_value = 0;
	unsigned int bits_used_for_max = 0;
	fann_type current_max_value = 0;
#endif

#ifndef FIXEDFANN
	if(save_as_fixed)
	{
		/* save the version information */
		fprintf(conf, FANN_FIX_VERSION "\n");
	}
	else
	{
		/* save the version information */
		fprintf(conf, FANN_FLO_VERSION "\n");
	}
#else
	/* save the version information */
	fprintf(conf, FANN_FIX_VERSION "\n");
#endif

#ifndef FIXEDFANN
	if(save_as_fixed)
	{
		/* calculate the maximal possible shift value */

		for(layer_it = ann->first_layer + 1; layer_it != ann->last_layer; layer_it++)
		{
			for(neuron_it = layer_it->first_neuron; neuron_it != layer_it->last_neuron; neuron_it++)
			{
				/* look at all connections to each neurons, and see how high a value we can get */
				current_max_value = 0;
				for(i = neuron_it->first_con; i != neuron_it->last_con; i++)
				{
					current_max_value += fann_abs(ann->weights[i]);
				}

				if(current_max_value > max_possible_value)
				{
					max_possible_value = current_max_value;
				}
			}
		}

		for(bits_used_for_max = 0; max_possible_value >= 1; bits_used_for_max++)
		{
			max_possible_value /= 2.0;
		}

		/* The maximum number of bits we shift the fix point, is the number
		 * of bits in a integer, minus one for the sign, one for the minus
		 * in stepwise, and minus the bits used for the maximum.
		 * This is devided by two, to allow multiplication of two fixed
		 * point numbers.
		 */
		calculated_decimal_point = (sizeof(int) * 8 - 2 - bits_used_for_max) / 2;

		if(calculated_decimal_point < 0)
		{
			decimal_point = 0;
		}
		else
		{
			decimal_point = calculated_decimal_point;
		}

		fixed_multiplier = 1 << decimal_point;

#ifdef DEBUG
		printf("calculated_decimal_point=%d, decimal_point=%u, bits_used_for_max=%u\n",
			   calculated_decimal_point, decimal_point, bits_used_for_max);
#endif

		/* save the decimal_point on a seperate line */
		fprintf(conf, "decimal_point=%u\n", decimal_point);
	}
#else
	/* save the decimal_point on a seperate line */
	fprintf(conf, "decimal_point=%u\n", ann->decimal_point);

#endif

	/* Save network parameters */
	fprintf(conf, "num_layers=%d\n", (int)(ann->last_layer - ann->first_layer));
	fprintf(conf, "learning_rate=%f\n", ann->learning_rate);
	fprintf(conf, "connection_rate=%f\n", ann->connection_rate);
	fprintf(conf, "network_type=%u\n", ann->network_type);

	fprintf(conf, "learning_momentum=%f\n", ann->learning_momentum);
	fprintf(conf, "training_algorithm=%u\n", ann->training_algorithm);
	fprintf(conf, "train_error_function=%u\n", ann->train_error_function);
	fprintf(conf, "train_stop_function=%u\n", ann->train_stop_function);
	fprintf(conf, "cascade_output_change_fraction=%f\n", ann->cascade_output_change_fraction);
	fprintf(conf, "quickprop_decay=%f\n", ann->quickprop_decay);
	fprintf(conf, "quickprop_mu=%f\n", ann->quickprop_mu);
	fprintf(conf, "rprop_increase_factor=%f\n", ann->rprop_increase_factor);
	fprintf(conf, "rprop_decrease_factor=%f\n", ann->rprop_decrease_factor);
	fprintf(conf, "rprop_delta_min=%f\n", ann->rprop_delta_min);
	fprintf(conf, "rprop_delta_max=%f\n", ann->rprop_delta_max);
	fprintf(conf, "rprop_delta_zero=%f\n", ann->rprop_delta_zero);
	fprintf(conf, "cascade_output_stagnation_epochs=%u\n", ann->cascade_output_stagnation_epochs);
	fprintf(conf, "cascade_candidate_change_fraction=%f\n", ann->cascade_candidate_change_fraction);
	fprintf(conf, "cascade_candidate_stagnation_epochs=%u\n", ann->cascade_candidate_stagnation_epochs);
	fprintf(conf, "cascade_max_out_epochs=%u\n", ann->cascade_max_out_epochs);
	fprintf(conf, "cascade_min_out_epochs=%u\n", ann->cascade_min_out_epochs);
	fprintf(conf, "cascade_max_cand_epochs=%u\n", ann->cascade_max_cand_epochs);
	fprintf(conf, "cascade_min_cand_epochs=%u\n", ann->cascade_min_cand_epochs);
	fprintf(conf, "cascade_num_candidate_groups=%u\n", ann->cascade_num_candidate_groups);

#ifndef FIXEDFANN
	if(save_as_fixed)
	{
		fprintf(conf, "bit_fail_limit=%u\n", (int) floor((ann->bit_fail_limit * fixed_multiplier) + 0.5));
		fprintf(conf, "cascade_candidate_limit=%u\n", (int) floor((ann->cascade_candidate_limit * fixed_multiplier) + 0.5));
		fprintf(conf, "cascade_weight_multiplier=%u\n", (int) floor((ann->cascade_weight_multiplier * fixed_multiplier) + 0.5));
	}
	else
#endif
	{
		fprintf(conf, "bit_fail_limit=" FANNPRINTF"\n", ann->bit_fail_limit);
		fprintf(conf, "cascade_candidate_limit=" FANNPRINTF"\n", ann->cascade_candidate_limit);
		fprintf(conf, "cascade_weight_multiplier=" FANNPRINTF"\n", ann->cascade_weight_multiplier);
	}

	fprintf(conf, "cascade_activation_functions_count=%u\n", ann->cascade_activation_functions_count);
	fprintf(conf, "cascade_activation_functions=");
	for(i = 0; i < ann->cascade_activation_functions_count; i++)
		fprintf(conf, "%u ", ann->cascade_activation_functions[i]);
	fprintf(conf, "\n");

	fprintf(conf, "cascade_activation_steepnesses_count=%u\n", ann->cascade_activation_steepnesses_count);
	fprintf(conf, "cascade_activation_steepnesses=");
	for(i = 0; i < ann->cascade_activation_steepnesses_count; i++)
	{
#ifndef FIXEDFANN
		if(save_as_fixed)
			fprintf(conf, "%u ", (int) floor((ann->cascade_activation_steepnesses[i] * fixed_multiplier) + 0.5));
		else
#endif
			fprintf(conf, FANNPRINTF" ", ann->cascade_activation_steepnesses[i]);
	}
	fprintf(conf, "\n");

	fprintf(conf, "layer_sizes=");
	for(layer_it = ann->first_layer; layer_it != ann->last_layer; layer_it++)
	{
		/* the number of neurons in the layers (in the last layer, there is always one too many neurons, because of an unused bias) */
		fprintf(conf, "%d ", (int)(layer_it->last_neuron - layer_it->first_neuron));
	}
	fprintf(conf, "\n");

#ifndef FIXEDFANN
	/* 2.1 */
	#define SCALE_SAVE( what, where )										\
		fprintf( conf, #what "_" #where "=" );								\
		for( i = 0; i < ann->num_##where##put; i++ )						\
			fprintf( conf, "%f ", ann->what##_##where[ i ] );				\
		fprintf( conf, "\n" );

	if(!save_as_fixed)
	{
		if(ann->scale_mean_in != NULL)
		{
			fprintf(conf, "scale_included=1\n");
			SCALE_SAVE( scale_mean,			in )
			SCALE_SAVE( scale_deviation,	in )
			SCALE_SAVE( scale_new_min,		in )
			SCALE_SAVE( scale_factor,		in )

			SCALE_SAVE( scale_mean,			out )
			SCALE_SAVE( scale_deviation,	out )
			SCALE_SAVE( scale_new_min,		out )
			SCALE_SAVE( scale_factor,		out )
		}
		else
			fprintf(conf, "scale_included=0\n");
	}
#undef SCALE_SAVE
#endif

	/* 2.0 */
	fprintf(conf, "neurons (num_inputs, activation_function, activation_steepness)=");
	for(layer_it = ann->first_layer; layer_it != ann->last_layer; layer_it++)
	{
		/* the neurons */
		for(neuron_it = layer_it->first_neuron; neuron_it != layer_it->last_neuron; neuron_it++)
		{
#ifndef FIXEDFANN
			if(save_as_fixed)
			{
				fprintf(conf, "(%u, %u, %u) ", neuron_it->last_con - neuron_it->first_con,
						neuron_it->activation_function,
						(int) floor((neuron_it->activation_steepness * fixed_multiplier) + 0.5));
			}
			else
			{
				fprintf(conf, "(%u, %u, " FANNPRINTF ") ", neuron_it->last_con - neuron_it->first_con,
						neuron_it->activation_function, neuron_it->activation_steepness);
			}
#else
			fprintf(conf, "(%u, %u, " FANNPRINTF ") ", neuron_it->last_con - neuron_it->first_con,
					neuron_it->activation_function, neuron_it->activation_steepness);
#endif
		}
	}
	fprintf(conf, "\n");

	connected_neurons = ann->connections;
	weights = ann->weights;
	first_neuron = ann->first_layer->first_neuron;

	/* Now save all the connections.
	 * We only need to save the source and the weight,
	 * since the destination is given by the order.
	 *
	 * The weight is not saved binary due to differences
	 * in binary definition of floating point numbers.
	 * Especially an iPAQ does not use the same binary
	 * representation as an i386 machine.
	 */
	fprintf(conf, "connections (connected_to_neuron, weight)=");
	for(i = 0; i < ann->total_connections; i++)
	{
#ifndef FIXEDFANN
		if(save_as_fixed)
		{
			/* save the connection "(source weight) " */
			fprintf(conf, "(%d, %d) ",
					(int)(connected_neurons[i] - first_neuron),
					(int) floor((weights[i] * fixed_multiplier) + 0.5));
		}
		else
		{
			/* save the connection "(source weight) " */
			fprintf(conf, "(%d, " FANNPRINTF ") ", (int)(connected_neurons[i] - first_neuron), weights[i]);
		}
#else
		/* save the connection "(source weight) " */
		fprintf(conf, "(%d, " FANNPRINTF ") ", (int)(connected_neurons[i] - first_neuron), weights[i]);
#endif

	}
	fprintf(conf, "\n");

	return calculated_decimal_point;
}

struct fann *fann_create_from_fd_1_1(FILE * conf, const char *configuration_file);

#define fann_scanf(type, name, val) \
{ \
	if(fscanf(conf, name"=" type"\n", val) != 1) \
	{ \
		fann_error(NULL, FANN_E_CANT_READ_CONFIG, name, configuration_file); \
		fann_destroy(ann); \
		return NULL; \
	} \
}

#define fann_skip(name) \
{ \
	if(fscanf(conf, name) != 0) \
	{ \
		fann_error(NULL, FANN_E_CANT_READ_CONFIG, name, configuration_file); \
		fann_destroy(ann); \
		return NULL; \
	} \
}

/* INTERNAL FUNCTION
   Create a network from a configuration file descriptor.
 */
struct fann *fann_create_from_fd(FILE * conf, const char *configuration_file)
{
	unsigned int num_layers, layer_size, input_neuron, i, num_connections;
	unsigned int tmpVal;
#ifdef FIXEDFANN
	unsigned int decimal_point, multiplier;
#else
	unsigned int scale_included;
#endif
	struct fann_neuron *first_neuron, *neuron_it, *last_neuron, **connected_neurons;
	fann_type *weights;
	struct fann_layer *layer_it;
	struct fann *ann = NULL;

	char *read_version;

	read_version = (char *) calloc(strlen(FANN_CONF_VERSION "\n"), 1);
	if(read_version == NULL)
	{
		fann_error(NULL, FANN_E_CANT_ALLOCATE_MEM);
		return NULL;
	}

	if(fread(read_version, 1, strlen(FANN_CONF_VERSION "\n"), conf) == 1)
	{
	        fann_error(NULL, FANN_E_CANT_READ_CONFIG, "FANN_VERSION", configuration_file);
		return NULL;
	}

	/* compares the version information */
	if(strncmp(read_version, FANN_CONF_VERSION "\n", strlen(FANN_CONF_VERSION "\n")) != 0)
	{
#ifdef FIXEDFANN
		if(strncmp(read_version, "FANN_FIX_1.1\n", strlen("FANN_FIX_1.1\n")) == 0)
		{
#else
		if(strncmp(read_version, "FANN_FLO_1.1\n", strlen("FANN_FLO_1.1\n")) == 0)
		{
#endif
			free(read_version);
			return fann_create_from_fd_1_1(conf, configuration_file);
		}

#ifndef FIXEDFANN
		/* Maintain compatibility with 2.0 version that doesnt have scale parameters. */
		if(strncmp(read_version, "FANN_FLO_2.0\n", strlen("FANN_FLO_2.0\n")) != 0 &&
		   strncmp(read_version, "FANN_FLO_2.1\n", strlen("FANN_FLO_2.1\n")) != 0)
#else
		if(strncmp(read_version, "FANN_FIX_2.0\n", strlen("FANN_FIX_2.0\n")) != 0 &&
		   strncmp(read_version, "FANN_FIX_2.1\n", strlen("FANN_FIX_2.1\n")) != 0)
#endif
		{
			free(read_version);
			fann_error(NULL, FANN_E_WRONG_CONFIG_VERSION, configuration_file);

			return NULL;
		}
	}

	free(read_version);

#ifdef FIXEDFANN
	fann_scanf("%u", "decimal_point", &decimal_point);
	multiplier = 1 << decimal_point;
#endif

	fann_scanf("%u", "num_layers", &num_layers);

	ann = fann_allocate_structure(num_layers);
	if(ann == NULL)
	{
		return NULL;
	}

	fann_scanf("%f", "learning_rate", &ann->learning_rate);
	fann_scanf("%f", "connection_rate", &ann->connection_rate);
	fann_scanf("%u", "network_type", &tmpVal);
	ann->network_type = (enum fann_nettype_enum)tmpVal;
	fann_scanf("%f", "learning_momentum", &ann->learning_momentum);
	fann_scanf("%u", "training_algorithm", &tmpVal);
	ann->training_algorithm = (enum fann_train_enum)tmpVal;
	fann_scanf("%u", "train_error_function", &tmpVal);
	ann->train_error_function = (enum fann_errorfunc_enum)tmpVal;
	fann_scanf("%u", "train_stop_function", &tmpVal);
	ann->train_stop_function = (enum fann_stopfunc_enum)tmpVal;
	fann_scanf("%f", "cascade_output_change_fraction", &ann->cascade_output_change_fraction);
	fann_scanf("%f", "quickprop_decay", &ann->quickprop_decay);
	fann_scanf("%f", "quickprop_mu", &ann->quickprop_mu);
	fann_scanf("%f", "rprop_increase_factor", &ann->rprop_increase_factor);
	fann_scanf("%f", "rprop_decrease_factor", &ann->rprop_decrease_factor);
	fann_scanf("%f", "rprop_delta_min", &ann->rprop_delta_min);
	fann_scanf("%f", "rprop_delta_max", &ann->rprop_delta_max);
	fann_scanf("%f", "rprop_delta_zero", &ann->rprop_delta_zero);
	fann_scanf("%u", "cascade_output_stagnation_epochs", &ann->cascade_output_stagnation_epochs);
	fann_scanf("%f", "cascade_candidate_change_fraction", &ann->cascade_candidate_change_fraction);
	fann_scanf("%u", "cascade_candidate_stagnation_epochs", &ann->cascade_candidate_stagnation_epochs);
	fann_scanf("%u", "cascade_max_out_epochs", &ann->cascade_max_out_epochs);
	fann_scanf("%u", "cascade_min_out_epochs", &ann->cascade_min_out_epochs);
	fann_scanf("%u", "cascade_max_cand_epochs", &ann->cascade_max_cand_epochs);
	fann_scanf("%u", "cascade_min_cand_epochs", &ann->cascade_min_cand_epochs);
	fann_scanf("%u", "cascade_num_candidate_groups", &ann->cascade_num_candidate_groups);

	fann_scanf(FANNSCANF, "bit_fail_limit", &ann->bit_fail_limit);
	fann_scanf(FANNSCANF, "cascade_candidate_limit", &ann->cascade_candidate_limit);
	fann_scanf(FANNSCANF, "cascade_weight_multiplier", &ann->cascade_weight_multiplier);


	fann_scanf("%u", "cascade_activation_functions_count", &ann->cascade_activation_functions_count);

	/* reallocate mem */
	ann->cascade_activation_functions =
		(enum fann_activationfunc_enum *)realloc(ann->cascade_activation_functions,
		ann->cascade_activation_functions_count * sizeof(enum fann_activationfunc_enum));
	if(ann->cascade_activation_functions == NULL)
	{
		fann_error((struct fann_error*)ann, FANN_E_CANT_ALLOCATE_MEM);
		fann_destroy(ann);
		return NULL;
	}


	fann_skip("cascade_activation_functions=");
	for(i = 0; i < ann->cascade_activation_functions_count; i++)
	{
		if(fscanf(conf, "%u ", &tmpVal) != 1)
		{
			fann_error(NULL, FANN_E_CANT_READ_CONFIG, "cascade_activation_functions", configuration_file);
			fann_destroy(ann);
			return NULL;
		}
		ann->cascade_activation_functions[i] = (enum fann_activationfunc_enum)tmpVal;
	}

	fann_scanf("%u", "cascade_activation_steepnesses_count", &ann->cascade_activation_steepnesses_count);

	/* reallocate mem */
	ann->cascade_activation_steepnesses =
		(fann_type *)realloc(ann->cascade_activation_steepnesses,
		ann->cascade_activation_steepnesses_count * sizeof(fann_type));
	if(ann->cascade_activation_steepnesses == NULL)
	{
		fann_error((struct fann_error*)ann, FANN_E_CANT_ALLOCATE_MEM);
		fann_destroy(ann);
		return NULL;
	}

	fann_skip("cascade_activation_steepnesses=");
	for(i = 0; i < ann->cascade_activation_steepnesses_count; i++)
	{
		if(fscanf(conf, FANNSCANF" ", &ann->cascade_activation_steepnesses[i]) != 1)
		{
			fann_error(NULL, FANN_E_CANT_READ_CONFIG, "cascade_activation_steepnesses", configuration_file);
			fann_destroy(ann);
			return NULL;
		}
	}

#ifdef FIXEDFANN
	ann->decimal_point = decimal_point;
	ann->multiplier = multiplier;
#endif

#ifdef FIXEDFANN
	fann_update_stepwise(ann);
#endif

#ifdef DEBUG
	printf("creating network with %d layers\n", num_layers);
	printf("input\n");
#endif

	fann_skip("layer_sizes=");
	/* determine how many neurons there should be in each layer */
	for(layer_it = ann->first_layer; layer_it != ann->last_layer; layer_it++)
	{
		if(fscanf(conf, "%u ", &layer_size) != 1
			|| layer_size == 0 || layer_size > INT_MAX - ann->total_neurons)
		{
			fann_error((struct fann_error *) ann, FANN_E_CANT_READ_CONFIG, "layer_sizes", configuration_file);
			fann_destroy(ann);
			return NULL;
		}
		/* we do not allocate room here, but we make sure that
		 * last_neuron - first_neuron is the number of neurons */
		layer_it->first_neuron = NULL;
		layer_it->last_neuron = layer_it->first_neuron + layer_size;
		ann->total_neurons += layer_size;
#ifdef DEBUG
		if(ann->network_type == FANN_NETTYPE_SHORTCUT && layer_it != ann->first_layer)
		{
			printf("  layer       : %d neurons, 0 bias\n", layer_size);
		}
		else
		{
			printf("  layer       : %d neurons, 1 bias\n", layer_size - 1);
		}
#endif
	}

	ann->num_input = (unsigned int)(ann->first_layer->last_neuron - ann->first_layer->first_neuron - 1);
	ann->num_output = (unsigned int)((ann->last_layer - 1)->last_neuron - (ann->last_layer - 1)->first_neuron);
	if(ann->network_type == FANN_NETTYPE_LAYER)
	{
		/* one too many (bias) in the output layer */
		ann->num_output--;
	}

#ifndef FIXEDFANN
#define SCALE_LOAD( what, where )											\
	fann_skip( #what "_" #where "=" );									\
	for(i = 0; i < ann->num_##where##put; i++)								\
	{																		\
		if(fscanf( conf, "%f ", (float *)&ann->what##_##where[ i ] ) != 1)  \
		{																	\
			fann_error((struct fann_error *) ann, FANN_E_CANT_READ_CONFIG, #what "_" #where, configuration_file); \
			fann_destroy(ann); 												\
			return NULL;													\
		}																	\
	}

	if(fscanf(conf, "scale_included=%u\n", &scale_included) == 1 && scale_included == 1)
	{
		fann_allocate_scale(ann);
		SCALE_LOAD( scale_mean,			in )
		SCALE_LOAD( scale_deviation,	in )
		SCALE_LOAD( scale_new_min,		in )
		SCALE_LOAD( scale_factor,		in )

		SCALE_LOAD( scale_mean,			out )
		SCALE_LOAD( scale_deviation,	out )
		SCALE_LOAD( scale_new_min,		out )
		SCALE_LOAD( scale_factor,		out )
	}
#undef SCALE_LOAD
#endif

	/* allocate room for the actual neurons */
	fann_allocate_neurons(ann);
	if(ann->errno_f == FANN_E_CANT_ALLOCATE_MEM)
	{
		fann_destroy(ann);
		return NULL;
	}

	last_neuron = (ann->last_layer - 1)->last_neuron;
	fann_skip("neurons (num_inputs, activation_function, activation_steepness)=");
	for(neuron_it = ann->first_layer->first_neuron; neuron_it != last_neuron; neuron_it++)
	{
		if(fscanf
		   (conf, "(%u, %u, " FANNSCANF ") ", &num_connections, &tmpVal,
			&neuron_it->activation_steepness) != 3)
		{
			fann_error((struct fann_error *) ann, FANN_E_CANT_READ_NEURON, configuration_file);
			fann_destroy(ann);
			return NULL;
		}
		neuron_it->activation_function = (enum fann_activationfunc_enum)tmpVal;
		neuron_it->first_con = ann->total_connections;
		ann->total_connections += num_connections;
		neuron_it->last_con = ann->total_connections;
	}

	fann_allocate_connections(ann);
	if(ann->errno_f == FANN_E_CANT_ALLOCATE_MEM)
	{
		fann_destroy(ann);
		return NULL;
	}

	connected_neurons = ann->connections;
	weights = ann->weights;
	first_neuron = ann->first_layer->first_neuron;

	fann_skip("connections (connected_to_neuron, weight)=");
	for(i = 0; i < ann->total_connections; i++)
	{
		if(fscanf(conf, "(%u, " FANNSCANF ") ", &input_neuron, &weights[i]) != 2)
		{
			fann_error((struct fann_error *) ann, FANN_E_CANT_READ_CONNECTIONS, configuration_file);
			fann_destroy(ann);
			return NULL;
		}
		connected_neurons[i] = first_neuron + input_neuron;
	}

#ifdef DEBUG
	printf("output\n");
#endif
	return ann;
}


/* INTERNAL FUNCTION
   Create a network from a configuration file descriptor. (backward compatible read of version 1.1 files)
 */
struct fann *fann_create_from_fd_1_1(FILE * conf, const char *configuration_file)
{
	unsigned int num_layers, layer_size, input_neuron, i, network_type, num_connections;
	unsigned int activation_function_hidden, activation_function_output;
#ifdef FIXEDFANN
	unsigned int decimal_point, multiplier;
#endif
	fann_type activation_steepness_hidden, activation_steepness_output;
	float learning_rate, connection_rate;
	struct fann_neuron *first_neuron, *neuron_it, *last_neuron, **connected_neurons;
	fann_type *weights;
	struct fann_layer *layer_it;
	struct fann *ann;

#ifdef FIXEDFANN
	if(fscanf(conf, "%u\n", &decimal_point) != 1)
	{
		fann_error(NULL, FANN_E_CANT_READ_CONFIG, "decimal_point", configuration_file);
		return NULL;
	}
	multiplier = 1 << decimal_point;
#endif

	if(fscanf(conf, "%u %f %f %u %u %u " FANNSCANF " " FANNSCANF "\n", &num_layers, &learning_rate,
		&connection_rate, &network_type, &activation_function_hidden,
		&activation_function_output, &activation_steepness_hidden,
		&activation_steepness_output) != 8)
	{
		fann_error(NULL, FANN_E_CANT_READ_CONFIG, "parameters", configuration_file);
		return NULL;
	}

	ann = fann_allocate_structure(num_layers);
	if(ann == NULL)
	{
		return NULL;
	}
	ann->connection_rate = connection_rate;
	ann->network_type = (enum fann_nettype_enum)network_type;
	ann->learning_rate = learning_rate;

#ifdef FIXEDFANN
	ann->decimal_point = decimal_point;
	ann->multiplier = multiplier;
#endif

#ifdef FIXEDFANN
	fann_update_stepwise(ann);
#endif

#ifdef DEBUG
	printf("creating network with learning rate %f\n", learning_rate);
	printf("input\n");
#endif

	/* determine how many neurons there should be in each layer */
	for(layer_it = ann->first_layer; layer_it != ann->last_layer; layer_it++)
	{
		if(fscanf(conf, "%u ", &layer_size) != 1)
		{
			fann_error((struct fann_error *) ann, FANN_E_CANT_READ_NEURON, configuration_file);
			fann_destroy(ann);
			return NULL;
		}
		/* we do not allocate room here, but we make sure that
		 * last_neuron - first_neuron is the number of neurons */
		layer_it->first_neuron = NULL;
		layer_it->last_neuron = layer_it->first_neuron + layer_size;
		ann->total_neurons += layer_size;
#ifdef DEBUG
		if(ann->network_type == FANN_NETTYPE_SHORTCUT && layer_it != ann->first_layer)
		{
			printf("  layer       : %d neurons, 0 bias\n", layer_size);
		}
		else
		{
			printf("  layer       : %d neurons, 1 bias\n", layer_size - 1);
		}
#endif
	}

	ann->num_input = (unsigned int)(ann->first_layer->last_neuron - ann->first_layer->first_neuron - 1);
	ann->num_output = (unsigned int)((ann->last_layer - 1)->last_neuron - (ann->last_layer - 1)->first_neuron);
	if(ann->network_type == FANN_NETTYPE_LAYER)
	{
		/* one too many (bias) in the output layer */
		ann->num_output--;
	}

	/* allocate room for the actual neurons */
	fann_allocate_neurons(ann);
	if(ann->errno_f == FANN_E_CANT_ALLOCATE_MEM)
	{
		fann_destroy(ann);
		return NULL;
	}

	last_neuron = (ann->last_layer - 1)->last_neuron;
	for(neuron_it = ann->first_layer->first_neuron; neuron_it != last_neuron; neuron_it++)
	{
		if(fscanf(conf, "%u ", &num_connections) != 1)
		{
			fann_error((struct fann_error *) ann, FANN_E_CANT_READ_NEURON, configuration_file);
			fann_destroy(ann);
			return NULL;
		}
		neuron_it->first_con = ann->total_connections;
		ann->total_connections += num_connections;
		neuron_it->last_con = ann->total_connections;
	}

	fann_allocate_connections(ann);
	if(ann->errno_f == FANN_E_CANT_ALLOCATE_MEM)
	{
		fann_destroy(ann);
		return NULL;
	}

	connected_neurons = ann->connections;
	weights = ann->weights;
	first_neuron = ann->first_layer->first_neuron;

	for(i = 0; i < ann->total_connections; i++)
	{
		if(fscanf(conf, "(%u " FANNSCANF ") ", &input_neuron, &weights[i]) != 2)
		{
			fann_error((struct fann_error *) ann, FANN_E_CANT_READ_CONNECTIONS, configuration_file);
			fann_destroy(ann);
			return NULL;
		}
		connected_neurons[i] = first_neuron + input_neuron;
	}

	fann_set_activation_steepness_hidden(ann, activation_steepness_hidden);
	fann_set_activation_steepness_output(ann, activation_steepness_output);
	fann_set_activation_function_hidden(ann, (enum fann_activationfunc_enum)activation_function_hidden);
	fann_set_activation_function_output(ann, (enum fann_activationfunc_enum)activation_function_output);

#ifdef DEBUG
	printf("output\n");
#endif
	return ann;
}


/*
file https://github.com/kaizhu256/fann/blob/v2021.2.21/src/fann_train.c
*/
/*
  Fast Artificial Neural Network Library (fann)
  Copyright (C) 2003-2016 Steffen Nissen (steffen.fann@gmail.com)

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

// #include "config.h"
// #include "fann.h"

/*#define DEBUGTRAIN*/

#ifndef FIXEDFANN
/* INTERNAL FUNCTION
  Calculates the derived of a value, given an activation function
   and a steepness
*/
fann_type fann_activation_derived(unsigned int activation_function,
								  fann_type steepness, fann_type value, fann_type sum)
{
	switch (activation_function)
	{
		case FANN_LINEAR:
		case FANN_LINEAR_PIECE:
		case FANN_LINEAR_PIECE_SYMMETRIC:
			return (fann_type) fann_linear_derive(steepness, value);
		case FANN_SIGMOID:
		case FANN_SIGMOID_STEPWISE:
			value = fann_clip(value, 0.01f, 0.99f);
			return (fann_type) fann_sigmoid_derive(steepness, value);
		case FANN_SIGMOID_SYMMETRIC:
		case FANN_SIGMOID_SYMMETRIC_STEPWISE:
			value = fann_clip(value, -0.98f, 0.98f);
			return (fann_type) fann_sigmoid_symmetric_derive(steepness, value);
		case FANN_GAUSSIAN:
			/* value = fann_clip(value, 0.01f, 0.99f); */
			return (fann_type) fann_gaussian_derive(steepness, value, sum);
		case FANN_GAUSSIAN_SYMMETRIC:
			/* value = fann_clip(value, -0.98f, 0.98f); */
			return (fann_type) fann_gaussian_symmetric_derive(steepness, value, sum);
		case FANN_ELLIOT:
			value = fann_clip(value, 0.01f, 0.99f);
			return (fann_type) fann_elliot_derive(steepness, value, sum);
		case FANN_ELLIOT_SYMMETRIC:
			value = fann_clip(value, -0.98f, 0.98f);
			return (fann_type) fann_elliot_symmetric_derive(steepness, value, sum);
		case FANN_SIN_SYMMETRIC:
			return (fann_type) fann_sin_symmetric_derive(steepness, sum);
		case FANN_COS_SYMMETRIC:
			return (fann_type) fann_cos_symmetric_derive(steepness, sum);
		case FANN_SIN:
			return (fann_type) fann_sin_derive(steepness, sum);
		case FANN_COS:
			return (fann_type) fann_cos_derive(steepness, sum);
		case FANN_THRESHOLD:
			fann_error(NULL, FANN_E_CANT_TRAIN_ACTIVATION);
	}
	return 0;
}

/* INTERNAL FUNCTION
  Calculates the activation of a value, given an activation function
   and a steepness
*/
fann_type fann_activation(struct fann * ann, unsigned int activation_function, fann_type steepness,
						  fann_type value)
{
	value = fann_mult(steepness, value);
	fann_activation_switch(activation_function, value, value);
	return value;
}

/* Trains the network with the backpropagation algorithm.
 */
FANN_EXTERNAL void FANN_API fann_train(struct fann *ann, fann_type * input,
									   fann_type * desired_output)
{
	fann_run(ann, input);

	fann_compute_MSE(ann, desired_output);

	fann_backpropagate_MSE(ann);

	fann_update_weights(ann);
}
#endif


/* INTERNAL FUNCTION
   Helper function to update the MSE value and return a diff which takes symmetric functions into account
*/
fann_type fann_update_MSE(struct fann *ann, struct fann_neuron* neuron, fann_type neuron_diff)
{
	float neuron_diff2;

	switch (neuron->activation_function)
	{
		case FANN_LINEAR_PIECE_SYMMETRIC:
		case FANN_THRESHOLD_SYMMETRIC:
		case FANN_SIGMOID_SYMMETRIC:
		case FANN_SIGMOID_SYMMETRIC_STEPWISE:
		case FANN_ELLIOT_SYMMETRIC:
		case FANN_GAUSSIAN_SYMMETRIC:
		case FANN_SIN_SYMMETRIC:
		case FANN_COS_SYMMETRIC:
			neuron_diff /= (fann_type)2.0;
			break;
		case FANN_THRESHOLD:
		case FANN_LINEAR:
		case FANN_SIGMOID:
		case FANN_SIGMOID_STEPWISE:
		case FANN_GAUSSIAN:
		case FANN_GAUSSIAN_STEPWISE:
		case FANN_ELLIOT:
		case FANN_LINEAR_PIECE:
		case FANN_SIN:
		case FANN_COS:
			break;
	}

#ifdef FIXEDFANN
		neuron_diff2 =
			(neuron_diff / (float) ann->multiplier) * (neuron_diff / (float) ann->multiplier);
#else
		neuron_diff2 = (float) (neuron_diff * neuron_diff);
#endif

	ann->MSE_value += neuron_diff2;

	/*printf("neuron_diff %f = (%f - %f)[/2], neuron_diff2=%f, sum=%f, MSE_value=%f, num_MSE=%d\n", neuron_diff, *desired_output, neuron_value, neuron_diff2, last_layer_begin->sum, ann->MSE_value, ann->num_MSE); */
	if(fann_abs(neuron_diff) >= ann->bit_fail_limit)
	{
		ann->num_bit_fail++;
	}

	return neuron_diff;
}

/* Tests the network.
 */
FANN_EXTERNAL fann_type *FANN_API fann_test(struct fann *ann, fann_type * input,
											fann_type * desired_output)
{
	fann_type neuron_value;
	fann_type *output_begin = fann_run(ann, input);
	fann_type *output_it;
	const fann_type *output_end = output_begin + ann->num_output;
	fann_type neuron_diff;
	struct fann_neuron *output_neuron = (ann->last_layer - 1)->first_neuron;

	/* calculate the error */
	for(output_it = output_begin; output_it != output_end; output_it++)
	{
		neuron_value = *output_it;

		neuron_diff = (*desired_output - neuron_value);

		neuron_diff = fann_update_MSE(ann, output_neuron, neuron_diff);

		desired_output++;
		output_neuron++;

		ann->num_MSE++;
	}

	return output_begin;
}

/* get the mean square error.
 */
FANN_EXTERNAL float FANN_API fann_get_MSE(struct fann *ann)
{
	if(ann->num_MSE)
	{
		return ann->MSE_value / (float) ann->num_MSE;
	}
	else
	{
		return 0;
	}
}

FANN_EXTERNAL unsigned int FANN_API fann_get_bit_fail(struct fann *ann)
{
	return ann->num_bit_fail;
}

/* reset the mean square error.
 */
FANN_EXTERNAL void FANN_API fann_reset_MSE(struct fann *ann)
{
/*printf("resetMSE %d %f\n", ann->num_MSE, ann->MSE_value);*/
	ann->num_MSE = 0;
	ann->MSE_value = 0;
	ann->num_bit_fail = 0;
}

#ifndef FIXEDFANN

/* INTERNAL FUNCTION
    compute the error at the network output
	(usually, after forward propagation of a certain input vector, fann_run)
	the error is a sum of squares for all the output units
	also increments a counter because MSE is an average of such errors

	After this train_errors in the output layer will be set to:
	neuron_value_derived * (desired_output - neuron_value)
 */
void fann_compute_MSE(struct fann *ann, fann_type * desired_output)
{
	fann_type neuron_value, neuron_diff, *error_it = 0, *error_begin = 0;
	struct fann_neuron *last_layer_begin = (ann->last_layer - 1)->first_neuron;
	const struct fann_neuron *last_layer_end = last_layer_begin + ann->num_output;
	const struct fann_neuron *first_neuron = ann->first_layer->first_neuron;

	/* if no room allocated for the error variabels, allocate it now */
	if(ann->train_errors == NULL)
	{
		ann->train_errors = (fann_type *) calloc(ann->total_neurons, sizeof(fann_type));
		if(ann->train_errors == NULL)
		{
			fann_error((struct fann_error *) ann, FANN_E_CANT_ALLOCATE_MEM);
			return;
		}
	}
	else
	{
		/* clear the error variabels */
		memset(ann->train_errors, 0, (ann->total_neurons) * sizeof(fann_type));
	}
	error_begin = ann->train_errors;

#ifdef DEBUGTRAIN
	printf("\ncalculate errors\n");
#endif
	/* calculate the error and place it in the output layer */
	error_it = error_begin + (last_layer_begin - first_neuron);

	for(; last_layer_begin != last_layer_end; last_layer_begin++)
	{
		neuron_value = last_layer_begin->value;
		neuron_diff = *desired_output - neuron_value;

		neuron_diff = fann_update_MSE(ann, last_layer_begin, neuron_diff);

		if(ann->train_error_function)
		{						/* TODO make switch when more functions */
			if(neuron_diff < -.9999999)
				neuron_diff = -17.0;
			else if(neuron_diff > .9999999)
				neuron_diff = 17.0;
			else
				neuron_diff = (fann_type) log((1.0 + neuron_diff) / (1.0 - neuron_diff));
		}

		*error_it = fann_activation_derived(last_layer_begin->activation_function,
											last_layer_begin->activation_steepness, neuron_value,
											last_layer_begin->sum) * neuron_diff;

		desired_output++;
		error_it++;

		ann->num_MSE++;
	}
}

/* INTERNAL FUNCTION
   Propagate the error backwards from the output layer.

   After this the train_errors in the hidden layers will be:
   neuron_value_derived * sum(outgoing_weights * connected_neuron)
*/
void fann_backpropagate_MSE(struct fann *ann)
{
	fann_type tmp_error;
	unsigned int i;
	struct fann_layer *layer_it;
	struct fann_neuron *neuron_it, *last_neuron;
	struct fann_neuron **connections;

	fann_type *error_begin = ann->train_errors;
	fann_type *error_prev_layer;
	fann_type *weights;
	const struct fann_neuron *first_neuron = ann->first_layer->first_neuron;
	const struct fann_layer *second_layer = ann->first_layer + 1;
	struct fann_layer *last_layer = ann->last_layer;

	/* go through all the layers, from last to first.
	 * And propagate the error backwards */
	for(layer_it = last_layer - 1; layer_it > second_layer; --layer_it)
	{
		last_neuron = layer_it->last_neuron;

		/* for each connection in this layer, propagate the error backwards */
		if(ann->connection_rate >= 1)
		{
			if(ann->network_type == FANN_NETTYPE_LAYER)
			{
				error_prev_layer = error_begin + ((layer_it - 1)->first_neuron - first_neuron);
			}
			else
			{
				error_prev_layer = error_begin;
			}

			for(neuron_it = layer_it->first_neuron; neuron_it != last_neuron; neuron_it++)
			{

				tmp_error = error_begin[neuron_it - first_neuron];
				weights = ann->weights + neuron_it->first_con;
				for(i = neuron_it->last_con - neuron_it->first_con; i--;)
				{
					/*printf("i = %d\n", i);
					 * printf("error_prev_layer[%d] = %f\n", i, error_prev_layer[i]);
					 * printf("weights[%d] = %f\n", i, weights[i]); */
					error_prev_layer[i] += tmp_error * weights[i];
				}
			}
		}
		else
		{
			for(neuron_it = layer_it->first_neuron; neuron_it != last_neuron; neuron_it++)
			{

				tmp_error = error_begin[neuron_it - first_neuron];
				weights = ann->weights + neuron_it->first_con;
				connections = ann->connections + neuron_it->first_con;
				for(i = neuron_it->last_con - neuron_it->first_con; i--;)
				{
					error_begin[connections[i] - first_neuron] += tmp_error * weights[i];
				}
			}
		}

		/* then calculate the actual errors in the previous layer */
		error_prev_layer = error_begin + ((layer_it - 1)->first_neuron - first_neuron);
		last_neuron = (layer_it - 1)->last_neuron;

		for(neuron_it = (layer_it - 1)->first_neuron; neuron_it != last_neuron; neuron_it++)
		{
			*error_prev_layer *= fann_activation_derived(neuron_it->activation_function,
				neuron_it->activation_steepness, neuron_it->value, neuron_it->sum);
			error_prev_layer++;
		}

	}
}

/* INTERNAL FUNCTION
   Update weights for incremental training
*/
void fann_update_weights(struct fann *ann)
{
	struct fann_neuron *neuron_it, *last_neuron, *prev_neurons;
	fann_type tmp_error, delta_w, *weights;
	struct fann_layer *layer_it;
	unsigned int i;
	unsigned int num_connections;

	/* store some variabels local for fast access */
	const float learning_rate = ann->learning_rate;
    const float learning_momentum = ann->learning_momentum;
	struct fann_neuron *first_neuron = ann->first_layer->first_neuron;
	struct fann_layer *first_layer = ann->first_layer;
	const struct fann_layer *last_layer = ann->last_layer;
	fann_type *error_begin = ann->train_errors;
	fann_type *deltas_begin, *weights_deltas;

	/* if no room allocated for the deltas, allocate it now */
	if(ann->prev_weights_deltas == NULL)
	{
		ann->prev_weights_deltas =
			(fann_type *) calloc(ann->total_connections_allocated, sizeof(fann_type));
		if(ann->prev_weights_deltas == NULL)
		{
			fann_error((struct fann_error *) ann, FANN_E_CANT_ALLOCATE_MEM);
			return;
		}
	}

#ifdef DEBUGTRAIN
	printf("\nupdate weights\n");
#endif
	deltas_begin = ann->prev_weights_deltas;
	prev_neurons = first_neuron;
	for(layer_it = (first_layer + 1); layer_it != last_layer; layer_it++)
	{
#ifdef DEBUGTRAIN
		printf("layer[%d]\n", layer_it - first_layer);
#endif
		last_neuron = layer_it->last_neuron;
		if(ann->connection_rate >= 1)
		{
			if(ann->network_type == FANN_NETTYPE_LAYER)
			{
				prev_neurons = (layer_it - 1)->first_neuron;
			}
			for(neuron_it = layer_it->first_neuron; neuron_it != last_neuron; neuron_it++)
			{
				tmp_error = error_begin[neuron_it - first_neuron] * learning_rate;
				num_connections = neuron_it->last_con - neuron_it->first_con;
				weights = ann->weights + neuron_it->first_con;
				weights_deltas = deltas_begin + neuron_it->first_con;
				for(i = 0; i != num_connections; i++)
				{
					delta_w = tmp_error * prev_neurons[i].value + learning_momentum * weights_deltas[i];
					weights[i] += delta_w ;
					weights_deltas[i] = delta_w;
				}
			}
		}
		else
		{
			for(neuron_it = layer_it->first_neuron; neuron_it != last_neuron; neuron_it++)
			{
				tmp_error = error_begin[neuron_it - first_neuron] * learning_rate;
				num_connections = neuron_it->last_con - neuron_it->first_con;
				weights = ann->weights + neuron_it->first_con;
				weights_deltas = deltas_begin + neuron_it->first_con;
				for(i = 0; i != num_connections; i++)
				{
					delta_w = tmp_error * prev_neurons[i].value + learning_momentum * weights_deltas[i];
					weights[i] += delta_w;
					weights_deltas[i] = delta_w;
				}
			}
		}
	}
}

/* INTERNAL FUNCTION
   Update slopes for batch training
   layer_begin = ann->first_layer+1 and layer_end = ann->last_layer-1
   will update all slopes.

*/
void fann_update_slopes_batch(struct fann *ann, struct fann_layer *layer_begin,
							  struct fann_layer *layer_end)
{
	struct fann_neuron *neuron_it, *last_neuron, *prev_neurons, **connections;
	fann_type tmp_error;
	unsigned int i, num_connections;

	/* store some variabels local for fast access */
	struct fann_neuron *first_neuron = ann->first_layer->first_neuron;
	fann_type *error_begin = ann->train_errors;
	fann_type *slope_begin, *neuron_slope;

	/* if no room allocated for the slope variabels, allocate it now */
	if(ann->train_slopes == NULL)
	{
		ann->train_slopes =
			(fann_type *) calloc(ann->total_connections_allocated, sizeof(fann_type));
		if(ann->train_slopes == NULL)
		{
			fann_error((struct fann_error *) ann, FANN_E_CANT_ALLOCATE_MEM);
			return;
		}
	}

	if(layer_begin == NULL)
	{
		layer_begin = ann->first_layer + 1;
	}

	if(layer_end == NULL)
	{
		layer_end = ann->last_layer - 1;
	}

	slope_begin = ann->train_slopes;

#ifdef DEBUGTRAIN
	printf("\nupdate slopes\n");
#endif

	prev_neurons = first_neuron;

	for(; layer_begin <= layer_end; layer_begin++)
	{
#ifdef DEBUGTRAIN
		printf("layer[%d]\n", layer_begin - ann->first_layer);
#endif
		last_neuron = layer_begin->last_neuron;
		if(ann->connection_rate >= 1)
		{
			if(ann->network_type == FANN_NETTYPE_LAYER)
			{
				prev_neurons = (layer_begin - 1)->first_neuron;
			}

			for(neuron_it = layer_begin->first_neuron; neuron_it != last_neuron; neuron_it++)
			{
				tmp_error = error_begin[neuron_it - first_neuron];
				neuron_slope = slope_begin + neuron_it->first_con;
				num_connections = neuron_it->last_con - neuron_it->first_con;
				for(i = 0; i != num_connections; i++)
				{
					neuron_slope[i] += tmp_error * prev_neurons[i].value;
				}
			}
		}
		else
		{
			for(neuron_it = layer_begin->first_neuron; neuron_it != last_neuron; neuron_it++)
			{
				tmp_error = error_begin[neuron_it - first_neuron];
				neuron_slope = slope_begin + neuron_it->first_con;
				num_connections = neuron_it->last_con - neuron_it->first_con;
				connections = ann->connections + neuron_it->first_con;
				for(i = 0; i != num_connections; i++)
				{
					neuron_slope[i] += tmp_error * connections[i]->value;
				}
			}
		}
	}
}

/* INTERNAL FUNCTION
   Clears arrays used for training before a new training session.
   Also creates the arrays that do not exist yet.
 */
void fann_clear_train_arrays(struct fann *ann)
{
	unsigned int i;
	fann_type delta_zero;

	/* if no room allocated for the slope variabels, allocate it now
	 * (calloc clears mem) */
	if(ann->train_slopes == NULL)
	{
		ann->train_slopes =
			(fann_type *) calloc(ann->total_connections_allocated, sizeof(fann_type));
		if(ann->train_slopes == NULL)
		{
			fann_error((struct fann_error *) ann, FANN_E_CANT_ALLOCATE_MEM);
			return;
		}
	}
	else
	{
		memset(ann->train_slopes, 0, (ann->total_connections_allocated) * sizeof(fann_type));
	}

	/* if no room allocated for the variabels, allocate it now */
	if(ann->prev_steps == NULL)
	{
		ann->prev_steps = (fann_type *) malloc(ann->total_connections_allocated * sizeof(fann_type));
		if(ann->prev_steps == NULL)
		{
			fann_error((struct fann_error *) ann, FANN_E_CANT_ALLOCATE_MEM);
			return;
		}
	}

	if(ann->training_algorithm == FANN_TRAIN_RPROP)
	{
		delta_zero = ann->rprop_delta_zero;

		for(i = 0; i < ann->total_connections_allocated; i++)
			ann->prev_steps[i] = delta_zero;
	}
	else
	{
		memset(ann->prev_steps, 0, (ann->total_connections_allocated) * sizeof(fann_type));
	}

	/* if no room allocated for the variabels, allocate it now */
	if(ann->prev_train_slopes == NULL)
	{
		ann->prev_train_slopes =
			(fann_type *) calloc(ann->total_connections_allocated, sizeof(fann_type));
		if(ann->prev_train_slopes == NULL)
		{
			fann_error((struct fann_error *) ann, FANN_E_CANT_ALLOCATE_MEM);
			return;
		}
	}
	else
	{
		memset(ann->prev_train_slopes, 0, (ann->total_connections_allocated) * sizeof(fann_type));
	}
}

/* INTERNAL FUNCTION
   Update weights for batch training
 */
void fann_update_weights_batch(struct fann *ann, unsigned int num_data, unsigned int first_weight,
							   unsigned int past_end)
{
	fann_type *train_slopes = ann->train_slopes;
	fann_type *weights = ann->weights;
	const float epsilon = ann->learning_rate / num_data;
	unsigned int i = first_weight;

	for(; i != past_end; i++)
	{
		weights[i] += train_slopes[i] * epsilon;
		train_slopes[i] = 0.0;
	}
}

/* INTERNAL FUNCTION
   The quickprop training algorithm
 */
void fann_update_weights_quickprop(struct fann *ann, unsigned int num_data,
								   unsigned int first_weight, unsigned int past_end)
{
	fann_type *train_slopes = ann->train_slopes;
	fann_type *weights = ann->weights;
	fann_type *prev_steps = ann->prev_steps;
	fann_type *prev_train_slopes = ann->prev_train_slopes;

	fann_type w, prev_step, slope, prev_slope, next_step;

	float epsilon = ann->learning_rate / num_data;
	float decay = ann->quickprop_decay;	/*-0.0001;*/
	float mu = ann->quickprop_mu;	/*1.75; */
	float shrink_factor = (float) (mu / (1.0 + mu));

	unsigned int i = first_weight;

	for(; i != past_end; i++)
	{
		w = weights[i];
		prev_step = prev_steps[i];
		slope = train_slopes[i] + decay * w;
		prev_slope = prev_train_slopes[i];
		next_step = 0.0;

		/* The step must always be in direction opposite to the slope. */
		if(prev_step > 0.001)
		{
			/* If last step was positive...  */
			if(slope > 0.0) /*  Add in linear term if current slope is still positive. */
				next_step += epsilon * slope;

			/*If current slope is close to or larger than prev slope...  */
			if(slope > (shrink_factor * prev_slope))
				next_step += mu * prev_step;	/* Take maximum size negative step. */
			else
				next_step += prev_step * slope / (prev_slope - slope);	/* Else, use quadratic estimate. */
		}
		else if(prev_step < -0.001)
		{
			/* If last step was negative...  */
			if(slope < 0.0) /*  Add in linear term if current slope is still negative. */
				next_step += epsilon * slope;

			/* If current slope is close to or more neg than prev slope... */
			if(slope < (shrink_factor * prev_slope))
				next_step += mu * prev_step;	/* Take maximum size negative step. */
			else
				next_step += prev_step * slope / (prev_slope - slope);	/* Else, use quadratic estimate. */
		}
		else /* Last step was zero, so use only linear term. */
			next_step += epsilon * slope;

		/*
		if(next_step > 1000 || next_step < -1000)
		{
			printf("quickprop[%d] weight=%f, slope=%f, prev_slope=%f, next_step=%f, prev_step=%f\n",
				   i, weights[i], slope, prev_slope, next_step, prev_step);

			   if(next_step > 1000)
			   next_step = 1000;
			   else
			   next_step = -1000;
		}
    	*/

		/* update global data arrays */
		prev_steps[i] = next_step;

		w += next_step;

		if(w > 1500)
			weights[i] = 1500;
		else if(w < -1500)
			weights[i] = -1500;
		else
			weights[i] = w;

		/*weights[i] = w;*/

		prev_train_slopes[i] = slope;
		train_slopes[i] = 0.0;
	}
}

/* INTERNAL FUNCTION
   The iRprop- algorithm
*/
void fann_update_weights_irpropm(struct fann *ann, unsigned int first_weight, unsigned int past_end)
{
	fann_type *train_slopes = ann->train_slopes;
	fann_type *weights = ann->weights;
	fann_type *prev_steps = ann->prev_steps;
	fann_type *prev_train_slopes = ann->prev_train_slopes;

	fann_type prev_step, slope, prev_slope, next_step, same_sign;

	float increase_factor = ann->rprop_increase_factor;	/*1.2; */
	float decrease_factor = ann->rprop_decrease_factor;	/*0.5; */
	float delta_min = ann->rprop_delta_min;	/*0.0; */
	float delta_max = ann->rprop_delta_max;	/*50.0; */

	unsigned int i = first_weight;

	for(; i != past_end; i++)
	{
		prev_step = fann_max(prev_steps[i], (fann_type) 0.0001);	/* prev_step may not be zero because then the training will stop */
		slope = train_slopes[i];
		prev_slope = prev_train_slopes[i];

		same_sign = prev_slope * slope;

		if(same_sign >= 0.0)
			next_step = fann_min(prev_step * increase_factor, delta_max);
		else
		{
			next_step = fann_max(prev_step * decrease_factor, delta_min);
			slope = 0;
		}

		if(slope < 0)
		{
			weights[i] -= next_step;
			if(weights[i] < -1500)
				weights[i] = -1500;
		}
		else
		{
			weights[i] += next_step;
			if(weights[i] > 1500)
				weights[i] = 1500;
		}

		/*if(i == 2){
		 * printf("weight=%f, slope=%f, next_step=%f, prev_step=%f\n", weights[i], slope, next_step, prev_step);
		 * } */

		/* update global data arrays */
		prev_steps[i] = next_step;
		prev_train_slopes[i] = slope;
		train_slopes[i] = 0.0;
	}
}

/* INTERNAL FUNCTION
   The SARprop- algorithm
*/
void fann_update_weights_sarprop(struct fann *ann, unsigned int epoch, unsigned int first_weight, unsigned int past_end)
{
	fann_type *train_slopes = ann->train_slopes;
	fann_type *weights = ann->weights;
	fann_type *prev_steps = ann->prev_steps;
	fann_type *prev_train_slopes = ann->prev_train_slopes;

	fann_type prev_step, slope, prev_slope, next_step = 0, same_sign;

	/* These should be set from variables */
	float increase_factor = ann->rprop_increase_factor;	/*1.2; */
	float decrease_factor = ann->rprop_decrease_factor;	/*0.5; */
	/* TODO: why is delta_min 0.0 in iRprop? SARPROP uses 1x10^-6 (Braun and Riedmiller, 1993) */
	float delta_min = 0.000001f;
	float delta_max = ann->rprop_delta_max;	/*50.0; */
	float weight_decay_shift = ann->sarprop_weight_decay_shift; /* ld 0.01 = -6.644 */
	float step_error_threshold_factor = ann->sarprop_step_error_threshold_factor; /* 0.1 */
	float step_error_shift = ann->sarprop_step_error_shift; /* ld 3 = 1.585 */
	float T = ann->sarprop_temperature;
	float MSE = fann_get_MSE(ann);
	float RMSE = sqrtf(MSE);

	unsigned int i = first_weight;


	/* for all weights; TODO: are biases included? */
	for(; i != past_end; i++)
	{
		/* TODO: confirm whether 1x10^-6 == delta_min is really better */
		prev_step = fann_max(prev_steps[i], (fann_type) 0.000001);	/* prev_step may not be zero because then the training will stop */
		/* calculate SARPROP slope; TODO: better as new error function? (see SARPROP paper)*/
		slope = -train_slopes[i] - weights[i] * (fann_type)fann_exp2(-T * epoch + weight_decay_shift);

		/* TODO: is prev_train_slopes[i] 0.0 in the beginning? */
		prev_slope = prev_train_slopes[i];

		same_sign = prev_slope * slope;

		if(same_sign > 0.0)
		{
			next_step = fann_min(prev_step * increase_factor, delta_max);
			/* TODO: are the signs inverted? see differences between SARPROP paper and iRprop */
			if (slope < 0.0)
				weights[i] += next_step;
			else
				weights[i] -= next_step;
		}
		else if(same_sign < 0.0)
		{
			if(prev_step < step_error_threshold_factor * MSE)
				next_step = prev_step * decrease_factor + (float)rand() / RAND_MAX * RMSE * (fann_type)fann_exp2(-T * epoch + step_error_shift);
			else
				next_step = fann_max(prev_step * decrease_factor, delta_min);

			slope = 0.0;
		}
		else
		{
			if(slope < 0.0)
				weights[i] += prev_step;
			else
				weights[i] -= prev_step;
		}


		/*if(i == 2){
		 * printf("weight=%f, slope=%f, next_step=%f, prev_step=%f\n", weights[i], slope, next_step, prev_step);
		 * } */

		/* update global data arrays */
		prev_steps[i] = next_step;
		prev_train_slopes[i] = slope;
		train_slopes[i] = 0.0;
	}
}

#endif

FANN_GET_SET(enum fann_train_enum, training_algorithm)
FANN_GET_SET(float, learning_rate)

FANN_EXTERNAL void FANN_API fann_set_activation_function_hidden(struct fann *ann,
																enum fann_activationfunc_enum activation_function)
{
	struct fann_neuron *last_neuron, *neuron_it;
	struct fann_layer *layer_it;
	struct fann_layer *last_layer = ann->last_layer - 1;	/* -1 to not update the output layer */

	for(layer_it = ann->first_layer + 1; layer_it != last_layer; layer_it++)
	{
		last_neuron = layer_it->last_neuron;
		for(neuron_it = layer_it->first_neuron; neuron_it != last_neuron; neuron_it++)
		{
			neuron_it->activation_function = activation_function;
		}
	}
}

FANN_EXTERNAL struct fann_layer* FANN_API fann_get_layer(struct fann *ann, int layer)
{
	if(layer <= 0 || layer >= (ann->last_layer - ann->first_layer))
	{
		fann_error((struct fann_error *) ann, FANN_E_INDEX_OUT_OF_BOUND, layer);
		return NULL;
	}

	return ann->first_layer + layer;
}

FANN_EXTERNAL struct fann_neuron* FANN_API fann_get_neuron_layer(struct fann *ann, struct fann_layer* layer, int neuron)
{
	if(neuron >= (layer->last_neuron - layer->first_neuron))
	{
		fann_error((struct fann_error *) ann, FANN_E_INDEX_OUT_OF_BOUND, neuron);
		return NULL;
	}

	return layer->first_neuron + neuron;
}

FANN_EXTERNAL struct fann_neuron* FANN_API fann_get_neuron(struct fann *ann, unsigned int layer, int neuron)
{
	struct fann_layer *layer_it = fann_get_layer(ann, layer);
	if(layer_it == NULL)
		return NULL;
	return fann_get_neuron_layer(ann, layer_it, neuron);
}

FANN_EXTERNAL enum fann_activationfunc_enum FANN_API
    fann_get_activation_function(struct fann *ann, int layer, int neuron)
{
	struct fann_neuron* neuron_it = fann_get_neuron(ann, layer, neuron);
	if (neuron_it == NULL)
    {
		return (enum fann_activationfunc_enum)-1; /* layer or neuron out of bounds */
    }
    else
    {
	    return neuron_it->activation_function;
    }
}

FANN_EXTERNAL void FANN_API fann_set_activation_function(struct fann *ann,
																enum fann_activationfunc_enum
																activation_function,
																int layer,
																int neuron)
{
	struct fann_neuron* neuron_it = fann_get_neuron(ann, layer, neuron);
	if(neuron_it == NULL)
		return;

	neuron_it->activation_function = activation_function;
}

FANN_EXTERNAL void FANN_API fann_set_activation_function_layer(struct fann *ann,
																enum fann_activationfunc_enum
																activation_function,
																int layer)
{
	struct fann_neuron *last_neuron, *neuron_it;
	struct fann_layer *layer_it = fann_get_layer(ann, layer);

	if(layer_it == NULL)
		return;

	last_neuron = layer_it->last_neuron;
	for(neuron_it = layer_it->first_neuron; neuron_it != last_neuron; neuron_it++)
	{
		neuron_it->activation_function = activation_function;
	}
}


FANN_EXTERNAL void FANN_API fann_set_activation_function_output(struct fann *ann,
																enum fann_activationfunc_enum activation_function)
{
	struct fann_neuron *last_neuron, *neuron_it;
	struct fann_layer *last_layer = ann->last_layer - 1;

	last_neuron = last_layer->last_neuron;
	for(neuron_it = last_layer->first_neuron; neuron_it != last_neuron; neuron_it++)
	{
		neuron_it->activation_function = activation_function;
	}
}

FANN_EXTERNAL void FANN_API fann_set_activation_steepness_hidden(struct fann *ann,
																 fann_type steepness)
{
	struct fann_neuron *last_neuron, *neuron_it;
	struct fann_layer *layer_it;
	struct fann_layer *last_layer = ann->last_layer - 1;	/* -1 to not update the output layer */

	for(layer_it = ann->first_layer + 1; layer_it != last_layer; layer_it++)
	{
		last_neuron = layer_it->last_neuron;
		for(neuron_it = layer_it->first_neuron; neuron_it != last_neuron; neuron_it++)
		{
			neuron_it->activation_steepness = steepness;
		}
	}
}

FANN_EXTERNAL fann_type FANN_API
    fann_get_activation_steepness(struct fann *ann, int layer, int neuron)
{
	struct fann_neuron* neuron_it = fann_get_neuron(ann, layer, neuron);
	if(neuron_it == NULL)
    {
		return -1; /* layer or neuron out of bounds */
    }
    else
    {
        return neuron_it->activation_steepness;
    }
}

FANN_EXTERNAL void FANN_API fann_set_activation_steepness(struct fann *ann,
																fann_type steepness,
																int layer,
																int neuron)
{
	struct fann_neuron* neuron_it = fann_get_neuron(ann, layer, neuron);
	if(neuron_it == NULL)
		return;

	neuron_it->activation_steepness = steepness;
}

FANN_EXTERNAL void FANN_API fann_set_activation_steepness_layer(struct fann *ann,
																fann_type steepness,
																int layer)
{
	struct fann_neuron *last_neuron, *neuron_it;
	struct fann_layer *layer_it = fann_get_layer(ann, layer);

	if(layer_it == NULL)
		return;

	last_neuron = layer_it->last_neuron;
	for(neuron_it = layer_it->first_neuron; neuron_it != last_neuron; neuron_it++)
	{
		neuron_it->activation_steepness = steepness;
	}
}

FANN_EXTERNAL void FANN_API fann_set_activation_steepness_output(struct fann *ann,
																 fann_type steepness)
{
	struct fann_neuron *last_neuron, *neuron_it;
	struct fann_layer *last_layer = ann->last_layer - 1;

	last_neuron = last_layer->last_neuron;
	for(neuron_it = last_layer->first_neuron; neuron_it != last_neuron; neuron_it++)
	{
		neuron_it->activation_steepness = steepness;
	}
}

FANN_GET_SET(enum fann_errorfunc_enum, train_error_function)
FANN_GET_SET(fann_callback_type, callback)
FANN_GET_SET(float, quickprop_decay)
FANN_GET_SET(float, quickprop_mu)
FANN_GET_SET(float, rprop_increase_factor)
FANN_GET_SET(float, rprop_decrease_factor)
FANN_GET_SET(float, rprop_delta_min)
FANN_GET_SET(float, rprop_delta_max)
FANN_GET_SET(float, rprop_delta_zero)
FANN_GET_SET(float, sarprop_weight_decay_shift)
FANN_GET_SET(float, sarprop_step_error_threshold_factor)
FANN_GET_SET(float, sarprop_step_error_shift)
FANN_GET_SET(float, sarprop_temperature)
FANN_GET_SET(enum fann_stopfunc_enum, train_stop_function)
FANN_GET_SET(fann_type, bit_fail_limit)
FANN_GET_SET(float, learning_momentum)


/*
file https://github.com/kaizhu256/fann/blob/v2021.2.21/src/fann_train_data.c
*/
/*
  Fast Artificial Neural Network Library (fann)
  Copyright (C) 2003-2016 Steffen Nissen (steffen.fann@gmail.com)

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

// #include "config.h"
// #include "fann.h"

/*
 * Reads training data from a file.
 */
FANN_EXTERNAL struct fann_train_data *FANN_API fann_read_train_from_file(const char *configuration_file)
{
	struct fann_train_data *data;
	FILE *file = fopen(configuration_file, "r");

	if(!file)
	{
		fann_error(NULL, FANN_E_CANT_OPEN_CONFIG_R, configuration_file);
		return NULL;
	}

	data = fann_read_train_from_fd(file, configuration_file);
	fclose(file);
	return data;
}

/*
 * Save training data to a file
 */
FANN_EXTERNAL int FANN_API fann_save_train(struct fann_train_data *data, const char *filename)
{
	return fann_save_train_internal(data, filename, 0, 0);
}

/*
 * Save training data to a file in fixed point algebra. (Good for testing
 * a network in fixed point)
 */
FANN_EXTERNAL int FANN_API fann_save_train_to_fixed(struct fann_train_data *data, const char *filename,
													 unsigned int decimal_point)
{
	return fann_save_train_internal(data, filename, 1, decimal_point);
}

/*
 * deallocate the train data structure.
 */
FANN_EXTERNAL void FANN_API fann_destroy_train(struct fann_train_data *data)
{
	if(data == NULL)
		return;
	if(data->input != NULL)
		fann_safe_free(data->input[0]);
	if(data->output != NULL)
		fann_safe_free(data->output[0]);
	fann_safe_free(data->input);
	fann_safe_free(data->output);
	fann_safe_free(data);
}

/*
 * Test a set of training data and calculate the MSE
 */
FANN_EXTERNAL float FANN_API fann_test_data(struct fann *ann, struct fann_train_data *data)
{
	unsigned int i;
	if(fann_check_input_output_sizes(ann, data) == -1)
		return 0;

	fann_reset_MSE(ann);

	for(i = 0; i != data->num_data; i++)
	{
		fann_test(ann, data->input[i], data->output[i]);
	}

	return fann_get_MSE(ann);
}

#ifndef FIXEDFANN

/*
 * Internal train function
 */
float fann_train_epoch_quickprop(struct fann *ann, struct fann_train_data *data)
{
	unsigned int i;

	if(ann->prev_train_slopes == NULL)
	{
		fann_clear_train_arrays(ann);
	}

	fann_reset_MSE(ann);

	for(i = 0; i < data->num_data; i++)
	{
		fann_run(ann, data->input[i]);
		fann_compute_MSE(ann, data->output[i]);
		fann_backpropagate_MSE(ann);
		fann_update_slopes_batch(ann, ann->first_layer + 1, ann->last_layer - 1);
	}
	fann_update_weights_quickprop(ann, data->num_data, 0, ann->total_connections);

	return fann_get_MSE(ann);
}

/*
 * Internal train function
 */
float fann_train_epoch_irpropm(struct fann *ann, struct fann_train_data *data)
{
	unsigned int i;

	if(ann->prev_train_slopes == NULL)
	{
		fann_clear_train_arrays(ann);
	}

	fann_reset_MSE(ann);

	for(i = 0; i < data->num_data; i++)
	{
		fann_run(ann, data->input[i]);
		fann_compute_MSE(ann, data->output[i]);
		fann_backpropagate_MSE(ann);
		fann_update_slopes_batch(ann, ann->first_layer + 1, ann->last_layer - 1);
	}

	fann_update_weights_irpropm(ann, 0, ann->total_connections);

	return fann_get_MSE(ann);
}

/*
 * Internal train function
 */
float fann_train_epoch_sarprop(struct fann *ann, struct fann_train_data *data)
{
	unsigned int i;

	if(ann->prev_train_slopes == NULL)
	{
		fann_clear_train_arrays(ann);
	}

	fann_reset_MSE(ann);

	for(i = 0; i < data->num_data; i++)
	{
		fann_run(ann, data->input[i]);
		fann_compute_MSE(ann, data->output[i]);
		fann_backpropagate_MSE(ann);
		fann_update_slopes_batch(ann, ann->first_layer + 1, ann->last_layer - 1);
	}

	fann_update_weights_sarprop(ann, ann->sarprop_epoch, 0, ann->total_connections);

	++(ann->sarprop_epoch);

	return fann_get_MSE(ann);
}

/*
 * Internal train function
 */
float fann_train_epoch_batch(struct fann *ann, struct fann_train_data *data)
{
	unsigned int i;

	fann_reset_MSE(ann);

	for(i = 0; i < data->num_data; i++)
	{
		fann_run(ann, data->input[i]);
		fann_compute_MSE(ann, data->output[i]);
		fann_backpropagate_MSE(ann);
		fann_update_slopes_batch(ann, ann->first_layer + 1, ann->last_layer - 1);
	}

	fann_update_weights_batch(ann, data->num_data, 0, ann->total_connections);

	return fann_get_MSE(ann);
}

/*
 * Internal train function
 */
float fann_train_epoch_incremental(struct fann *ann, struct fann_train_data *data)
{
	unsigned int i;

	fann_reset_MSE(ann);

	for(i = 0; i != data->num_data; i++)
	{
		fann_train(ann, data->input[i], data->output[i]);
	}

	return fann_get_MSE(ann);
}

/*
 * Train for one epoch with the selected training algorithm
 */
FANN_EXTERNAL float FANN_API fann_train_epoch(struct fann *ann, struct fann_train_data *data)
{
	if(fann_check_input_output_sizes(ann, data) == -1)
		return 0;

	switch (ann->training_algorithm)
	{
	case FANN_TRAIN_QUICKPROP:
		return fann_train_epoch_quickprop(ann, data);
	case FANN_TRAIN_RPROP:
		return fann_train_epoch_irpropm(ann, data);
	case FANN_TRAIN_SARPROP:
		return fann_train_epoch_sarprop(ann, data);
	case FANN_TRAIN_BATCH:
		return fann_train_epoch_batch(ann, data);
	case FANN_TRAIN_INCREMENTAL:
		return fann_train_epoch_incremental(ann, data);
	}
	return 0;
}

FANN_EXTERNAL void FANN_API fann_train_on_data(struct fann *ann, struct fann_train_data *data,
											   unsigned int max_epochs,
											   unsigned int epochs_between_reports,
											   float desired_error)
{
	float error;
	unsigned int i;
	int desired_error_reached;

#ifdef DEBUG
	printf("Training with %s\n", FANN_TRAIN_NAMES[ann->training_algorithm]);
#endif

	if(epochs_between_reports && ann->callback == NULL)
	{
		printf("Max epochs %8d. Desired error: %.10f.\n", max_epochs, desired_error);
	}

	for(i = 1; i <= max_epochs; i++)
	{
		/*
		 * train
		 */
		error = fann_train_epoch(ann, data);
		desired_error_reached = fann_desired_error_reached(ann, desired_error);

		/*
		 * print current output
		 */
		if(epochs_between_reports &&
		   (i % epochs_between_reports == 0 || i == max_epochs || i == 1 ||
			desired_error_reached == 0))
		{
			if(ann->callback == NULL)
			{
				printf("Epochs     %8d. Current error: %.10f. Bit fail %d.\n", i, error,
					   ann->num_bit_fail);
			}
			else if(((*ann->callback)(ann, data, max_epochs, epochs_between_reports,
									  desired_error, i)) == -1)
			{
				/*
				 * you can break the training by returning -1
				 */
				break;
			}
		}

		if(desired_error_reached == 0)
			break;
	}
}

FANN_EXTERNAL void FANN_API fann_train_on_file(struct fann *ann, const char *filename,
											   unsigned int max_epochs,
											   unsigned int epochs_between_reports,
											   float desired_error)
{
	struct fann_train_data *data = fann_read_train_from_file(filename);

	if(data == NULL)
	{
		return;
	}
	fann_train_on_data(ann, data, max_epochs, epochs_between_reports, desired_error);
	fann_destroy_train(data);
}

#endif

/*
 * shuffles training data, randomizing the order
 */
FANN_EXTERNAL void FANN_API fann_shuffle_train_data(struct fann_train_data *train_data)
{
	unsigned int dat = 0, elem, swap;
	fann_type temp;

	for(; dat < train_data->num_data; dat++)
	{
		swap = (unsigned int) (rand() % train_data->num_data);
		if(swap != dat)
		{
			for(elem = 0; elem < train_data->num_input; elem++)
			{
				temp = train_data->input[dat][elem];
				train_data->input[dat][elem] = train_data->input[swap][elem];
				train_data->input[swap][elem] = temp;
			}
			for(elem = 0; elem < train_data->num_output; elem++)
			{
				temp = train_data->output[dat][elem];
				train_data->output[dat][elem] = train_data->output[swap][elem];
				train_data->output[swap][elem] = temp;
			}
		}
	}
}

/*
 * INTERNAL FUNCTION calculates min and max of train data
 */
void fann_get_min_max_data(fann_type ** data, unsigned int num_data, unsigned int num_elem, fann_type *min, fann_type *max)
{
	fann_type temp;
	unsigned int dat, elem;
	*min = *max = data[0][0];

	for(dat = 0; dat < num_data; dat++)
	{
		for(elem = 0; elem < num_elem; elem++)
		{
			temp = data[dat][elem];
			if(temp < *min)
				*min = temp;
			else if(temp > *max)
				*max = temp;
		}
	}
}


FANN_EXTERNAL fann_type FANN_API fann_get_min_train_input(struct fann_train_data *train_data)
{
    fann_type min, max;
    fann_get_min_max_data(train_data->input, train_data->num_data, train_data->num_input, &min, &max);
    return min;
}

FANN_EXTERNAL fann_type FANN_API fann_get_max_train_input(struct fann_train_data *train_data)
{
    fann_type min, max;
    fann_get_min_max_data(train_data->input, train_data->num_data, train_data->num_input, &min, &max);
    return max;
}

FANN_EXTERNAL fann_type FANN_API fann_get_min_train_output(struct fann_train_data *train_data)
{
    fann_type min, max;
    fann_get_min_max_data(train_data->output, train_data->num_data, train_data->num_output, &min, &max);
    return min;
}

FANN_EXTERNAL fann_type FANN_API fann_get_max_train_output(struct fann_train_data *train_data)
{
    fann_type min, max;
    fann_get_min_max_data(train_data->output, train_data->num_data, train_data->num_output, &min, &max);
    return max;
}

/*
 * INTERNAL FUNCTION Scales data to a specific range
 */
void fann_scale_data(fann_type ** data, unsigned int num_data, unsigned int num_elem,
					 fann_type new_min, fann_type new_max)
{
	fann_type old_min, old_max;
	fann_get_min_max_data(data, num_data, num_elem, &old_min, &old_max);
	fann_scale_data_to_range(data, num_data, num_elem, old_min, old_max, new_min, new_max);
}

/*
 * INTERNAL FUNCTION Scales data to a specific range
 */
FANN_EXTERNAL void FANN_API fann_scale_data_to_range(fann_type ** data, unsigned int num_data, unsigned int num_elem,
					 fann_type old_min, fann_type old_max, fann_type new_min, fann_type new_max)
{
	unsigned int dat, elem;
	fann_type temp, old_span, new_span, factor;

	old_span = old_max - old_min;
	new_span = new_max - new_min;
	factor = new_span / old_span;
	/*printf("max %f, min %f, factor %f\n", old_max, old_min, factor);*/

	for(dat = 0; dat < num_data; dat++)
	{
		for(elem = 0; elem < num_elem; elem++)
		{
			temp = (data[dat][elem] - old_min) * factor + new_min;
			if(temp < new_min)
			{
				data[dat][elem] = new_min;
				/*
				 * printf("error %f < %f\n", temp, new_min);
				 */
			}
			else if(temp > new_max)
			{
				data[dat][elem] = new_max;
				/*
				 * printf("error %f > %f\n", temp, new_max);
				 */
			}
			else
			{
				data[dat][elem] = temp;
			}
		}
	}
}


/*
 * Scales the inputs in the training data to the specified range
 */
FANN_EXTERNAL void FANN_API fann_scale_input_train_data(struct fann_train_data *train_data,
														fann_type new_min, fann_type new_max)
{
	fann_scale_data(train_data->input, train_data->num_data, train_data->num_input, new_min,
					new_max);
}

/*
 * Scales the inputs in the training data to the specified range
 */
FANN_EXTERNAL void FANN_API fann_scale_output_train_data(struct fann_train_data *train_data,
														 fann_type new_min, fann_type new_max)
{
	fann_scale_data(train_data->output, train_data->num_data, train_data->num_output, new_min,
					new_max);
}

/*
 * Scales the inputs in the training data to the specified range
 */
FANN_EXTERNAL void FANN_API fann_scale_train_data(struct fann_train_data *train_data,
												  fann_type new_min, fann_type new_max)
{
	fann_scale_data(train_data->input, train_data->num_data, train_data->num_input, new_min,
					new_max);
	fann_scale_data(train_data->output, train_data->num_data, train_data->num_output, new_min,
					new_max);
}

/*
 * merges training data into a single struct.
 */
FANN_EXTERNAL struct fann_train_data *FANN_API fann_merge_train_data(struct fann_train_data *data1,
																	 struct fann_train_data *data2)
{
	unsigned int i;
	fann_type *data_input, *data_output;
	struct fann_train_data *dest =
		(struct fann_train_data *) malloc(sizeof(struct fann_train_data));

	if(dest == NULL)
	{
		fann_error((struct fann_error*)data1, FANN_E_CANT_ALLOCATE_MEM);
		return NULL;
	}

	if((data1->num_input != data2->num_input) || (data1->num_output != data2->num_output))
	{
		fann_error((struct fann_error*)data1, FANN_E_TRAIN_DATA_MISMATCH);
		return NULL;
	}

	fann_init_error_data((struct fann_error *) dest);
	dest->error_log = data1->error_log;

	dest->num_data = data1->num_data+data2->num_data;
	dest->num_input = data1->num_input;
	dest->num_output = data1->num_output;
	dest->input = (fann_type **) calloc(dest->num_data, sizeof(fann_type *));
	if(dest->input == NULL)
	{
		fann_error((struct fann_error*)data1, FANN_E_CANT_ALLOCATE_MEM);
		fann_destroy_train(dest);
		return NULL;
	}

	dest->output = (fann_type **) calloc(dest->num_data, sizeof(fann_type *));
	if(dest->output == NULL)
	{
		fann_error((struct fann_error*)data1, FANN_E_CANT_ALLOCATE_MEM);
		fann_destroy_train(dest);
		return NULL;
	}

	data_input = (fann_type *) calloc(dest->num_input * dest->num_data, sizeof(fann_type));
	if(data_input == NULL)
	{
		fann_error((struct fann_error*)data1, FANN_E_CANT_ALLOCATE_MEM);
		fann_destroy_train(dest);
		return NULL;
	}
	memcpy(data_input, data1->input[0], dest->num_input * data1->num_data * sizeof(fann_type));
	memcpy(data_input + (dest->num_input*data1->num_data),
		data2->input[0], dest->num_input * data2->num_data * sizeof(fann_type));

	data_output = (fann_type *) calloc(dest->num_output * dest->num_data, sizeof(fann_type));
	if(data_output == NULL)
	{
		fann_error((struct fann_error*)data1, FANN_E_CANT_ALLOCATE_MEM);
		fann_destroy_train(dest);
		return NULL;
	}
	memcpy(data_output, data1->output[0], dest->num_output * data1->num_data * sizeof(fann_type));
	memcpy(data_output + (dest->num_output*data1->num_data),
		data2->output[0], dest->num_output * data2->num_data * sizeof(fann_type));

	for(i = 0; i != dest->num_data; i++)
	{
		dest->input[i] = data_input;
		data_input += dest->num_input;
		dest->output[i] = data_output;
		data_output += dest->num_output;
	}
	return dest;
}

/*
 * return a copy of a fann_train_data struct
 */
FANN_EXTERNAL struct fann_train_data *FANN_API fann_duplicate_train_data(struct fann_train_data
																		 *data)
{
	unsigned int i;
	fann_type *data_input, *data_output;
	struct fann_train_data *dest =
		(struct fann_train_data *) malloc(sizeof(struct fann_train_data));

	if(dest == NULL)
	{
		fann_error((struct fann_error*)data, FANN_E_CANT_ALLOCATE_MEM);
		return NULL;
	}

	fann_init_error_data((struct fann_error *) dest);
	dest->error_log = data->error_log;

	dest->num_data = data->num_data;
	dest->num_input = data->num_input;
	dest->num_output = data->num_output;
	dest->input = (fann_type **) calloc(dest->num_data, sizeof(fann_type *));
	if(dest->input == NULL)
	{
		fann_error((struct fann_error*)data, FANN_E_CANT_ALLOCATE_MEM);
		fann_destroy_train(dest);
		return NULL;
	}

	dest->output = (fann_type **) calloc(dest->num_data, sizeof(fann_type *));
	if(dest->output == NULL)
	{
		fann_error((struct fann_error*)data, FANN_E_CANT_ALLOCATE_MEM);
		fann_destroy_train(dest);
		return NULL;
	}

	data_input = (fann_type *) calloc(dest->num_input * dest->num_data, sizeof(fann_type));
	if(data_input == NULL)
	{
		fann_error((struct fann_error*)data, FANN_E_CANT_ALLOCATE_MEM);
		fann_destroy_train(dest);
		return NULL;
	}
	memcpy(data_input, data->input[0], dest->num_input * dest->num_data * sizeof(fann_type));

	data_output = (fann_type *) calloc(dest->num_output * dest->num_data, sizeof(fann_type));
	if(data_output == NULL)
	{
		fann_error((struct fann_error*)data, FANN_E_CANT_ALLOCATE_MEM);
		fann_destroy_train(dest);
		return NULL;
	}
	memcpy(data_output, data->output[0], dest->num_output * dest->num_data * sizeof(fann_type));

	for(i = 0; i != dest->num_data; i++)
	{
		dest->input[i] = data_input;
		data_input += dest->num_input;
		dest->output[i] = data_output;
		data_output += dest->num_output;
	}
	return dest;
}

FANN_EXTERNAL struct fann_train_data *FANN_API fann_subset_train_data(struct fann_train_data
																		 *data, unsigned int pos,
																		 unsigned int length)
{
	unsigned int i;
	fann_type *data_input, *data_output;
	struct fann_train_data *dest =
		(struct fann_train_data *) malloc(sizeof(struct fann_train_data));

	if(dest == NULL)
	{
		fann_error((struct fann_error*)data, FANN_E_CANT_ALLOCATE_MEM);
		return NULL;
	}

	if(pos > data->num_data || pos+length > data->num_data)
	{
		fann_error((struct fann_error*)data, FANN_E_TRAIN_DATA_SUBSET, pos, length, data->num_data);
		return NULL;
	}

	fann_init_error_data((struct fann_error *) dest);
	dest->error_log = data->error_log;

	dest->num_data = length;
	dest->num_input = data->num_input;
	dest->num_output = data->num_output;
	dest->input = (fann_type **) calloc(dest->num_data, sizeof(fann_type *));
	if(dest->input == NULL)
	{
		fann_error((struct fann_error*)data, FANN_E_CANT_ALLOCATE_MEM);
		fann_destroy_train(dest);
		return NULL;
	}

	dest->output = (fann_type **) calloc(dest->num_data, sizeof(fann_type *));
	if(dest->output == NULL)
	{
		fann_error((struct fann_error*)data, FANN_E_CANT_ALLOCATE_MEM);
		fann_destroy_train(dest);
		return NULL;
	}

	data_input = (fann_type *) calloc(dest->num_input * dest->num_data, sizeof(fann_type));
	if(data_input == NULL)
	{
		fann_error((struct fann_error*)data, FANN_E_CANT_ALLOCATE_MEM);
		fann_destroy_train(dest);
		return NULL;
	}
	memcpy(data_input, data->input[pos], dest->num_input * dest->num_data * sizeof(fann_type));

	data_output = (fann_type *) calloc(dest->num_output * dest->num_data, sizeof(fann_type));
	if(data_output == NULL)
	{
		fann_error((struct fann_error*)data, FANN_E_CANT_ALLOCATE_MEM);
		fann_destroy_train(dest);
		return NULL;
	}
	memcpy(data_output, data->output[pos], dest->num_output * dest->num_data * sizeof(fann_type));

	for(i = 0; i != dest->num_data; i++)
	{
		dest->input[i] = data_input;
		data_input += dest->num_input;
		dest->output[i] = data_output;
		data_output += dest->num_output;
	}
	return dest;
}

FANN_EXTERNAL unsigned int FANN_API fann_length_train_data(struct fann_train_data *data)
{
	return data->num_data;
}

FANN_EXTERNAL unsigned int FANN_API fann_num_input_train_data(struct fann_train_data *data)
{
	return data->num_input;
}

FANN_EXTERNAL unsigned int FANN_API fann_num_output_train_data(struct fann_train_data *data)
{
	return data->num_output;
}

/* INTERNAL FUNCTION
   Save the train data structure.
 */
int fann_save_train_internal(struct fann_train_data *data, const char *filename,
							  unsigned int save_as_fixed, unsigned int decimal_point)
{
	int retval = 0;
	FILE *file = fopen(filename, "w");

	if(!file)
	{
		fann_error((struct fann_error *) data, FANN_E_CANT_OPEN_TD_W, filename);
		return -1;
	}
	retval = fann_save_train_internal_fd(data, file, filename, save_as_fixed, decimal_point);
	fclose(file);

	return retval;
}

/* INTERNAL FUNCTION
   Save the train data structure.
 */
int fann_save_train_internal_fd(struct fann_train_data *data, FILE * file, const char *filename,
								 unsigned int save_as_fixed, unsigned int decimal_point)
{
	unsigned int num_data = data->num_data;
	unsigned int num_input = data->num_input;
	unsigned int num_output = data->num_output;
	unsigned int i, j;
	int retval = 0;

#ifndef FIXEDFANN
	unsigned int multiplier = 1 << decimal_point;
#endif

	fprintf(file, "%u %u %u\n", data->num_data, data->num_input, data->num_output);

	for(i = 0; i < num_data; i++)
	{
		for(j = 0; j < num_input; j++)
		{
#ifndef FIXEDFANN
			if(save_as_fixed)
			{
				fprintf(file, "%d ", (int) (data->input[i][j] * multiplier));
			}
			else
			{
				if(((int) floor(data->input[i][j] + 0.5) * 1000000) ==
				   ((int) floor(data->input[i][j] * 1000000.0 + 0.5)))
				{
					fprintf(file, "%d ", (int) data->input[i][j]);
				}
				else
				{
					fprintf(file, "%f ", data->input[i][j]);
				}
			}
#else
			fprintf(file, FANNPRINTF " ", data->input[i][j]);
#endif
		}
		fprintf(file, "\n");

		for(j = 0; j < num_output; j++)
		{
#ifndef FIXEDFANN
			if(save_as_fixed)
			{
				fprintf(file, "%d ", (int) (data->output[i][j] * multiplier));
			}
			else
			{
				if(((int) floor(data->output[i][j] + 0.5) * 1000000) ==
				   ((int) floor(data->output[i][j] * 1000000.0 + 0.5)))
				{
					fprintf(file, "%d ", (int) data->output[i][j]);
				}
				else
				{
					fprintf(file, "%f ", data->output[i][j]);
				}
			}
#else
			fprintf(file, FANNPRINTF " ", data->output[i][j]);
#endif
		}
		fprintf(file, "\n");
	}

	return retval;
}

/*
 * Creates an empty set of training data
 */
FANN_EXTERNAL struct fann_train_data * FANN_API fann_create_train(unsigned int num_data, unsigned int num_input, unsigned int num_output)
{
	fann_type *data_input, *data_output;
	unsigned int i;
	struct fann_train_data *data =
		(struct fann_train_data *) malloc(sizeof(struct fann_train_data));

	if(data == NULL)
	{
		fann_error(NULL, FANN_E_CANT_ALLOCATE_MEM);
		return NULL;
	}

	fann_init_error_data((struct fann_error *) data);

	data->num_data = num_data;
	data->num_input = num_input;
	data->num_output = num_output;
	data->input = (fann_type **) calloc(num_data, sizeof(fann_type *));
	if(data->input == NULL)
	{
		fann_error(NULL, FANN_E_CANT_ALLOCATE_MEM);
		fann_destroy_train(data);
		return NULL;
	}

	data->output = (fann_type **) calloc(num_data, sizeof(fann_type *));
	if(data->output == NULL)
	{
		fann_error(NULL, FANN_E_CANT_ALLOCATE_MEM);
		fann_destroy_train(data);
		return NULL;
	}

	data_input = (fann_type *) calloc(num_input * num_data, sizeof(fann_type));
	if(data_input == NULL)
	{
		fann_error(NULL, FANN_E_CANT_ALLOCATE_MEM);
		fann_destroy_train(data);
		return NULL;
	}

	data_output = (fann_type *) calloc(num_output * num_data, sizeof(fann_type));
	if(data_output == NULL)
	{
		fann_error(NULL, FANN_E_CANT_ALLOCATE_MEM);
		fann_destroy_train(data);
		return NULL;
	}

	for(i = 0; i != num_data; i++)
	{
		data->input[i] = data_input;
		data_input += num_input;
		data->output[i] = data_output;
		data_output += num_output;
	}
	return data;
}

FANN_EXTERNAL struct fann_train_data * FANN_API fann_create_train_pointer_array(unsigned int num_data, unsigned int num_input, fann_type **input, unsigned int num_output, fann_type **output)
{
	unsigned int i;
    struct fann_train_data *data;
	data = fann_create_train(num_data, num_input, num_output);

	if(data == NULL)
		return NULL;

    for (i = 0; i < num_data; ++i)
    {
		memcpy(data->input[i], input[i], num_input*sizeof(fann_type));
		memcpy(data->output[i], output[i], num_output*sizeof(fann_type));
    }

	return data;
}

FANN_EXTERNAL struct fann_train_data * FANN_API fann_create_train_array(unsigned int num_data, unsigned int num_input, fann_type *input, unsigned int num_output, fann_type *output)
{
	unsigned int i;
    struct fann_train_data *data;
	data = fann_create_train(num_data, num_input, num_output);

	if(data == NULL)
		return NULL;

    for (i = 0; i < num_data; ++i)
    {
		memcpy(data->input[i], &input[i*num_input], num_input*sizeof(fann_type));
		memcpy(data->output[i], &output[i*num_output], num_output*sizeof(fann_type));
    }

	return data;
}


/*
 * Creates training data from a callback function.
 */
FANN_EXTERNAL struct fann_train_data * FANN_API fann_create_train_from_callback(unsigned int num_data,
                                          unsigned int num_input,
                                          unsigned int num_output,
                                          void (FANN_API *user_function)( unsigned int,
                                                                 unsigned int,
                                                                 unsigned int,
                                                                 fann_type * ,
                                                                 fann_type * ))
{
    unsigned int i;
	struct fann_train_data *data = fann_create_train(num_data, num_input, num_output);
	if(data == NULL)
	{
		return NULL;
	}

    for( i = 0; i != num_data; i++)
    {
        (*user_function)(i, num_input, num_output, data->input[i], data->output[i]);
    }

    return data;
}

FANN_EXTERNAL fann_type * FANN_API fann_get_train_input(struct fann_train_data * data, unsigned int position)
{
	if(position >= data->num_data)
		return NULL;
	return data->input[position];
}

FANN_EXTERNAL fann_type * FANN_API fann_get_train_output(struct fann_train_data * data, unsigned int position)
{
	if(position >= data->num_data)
		return NULL;
	return data->output[position];
}


/*
 * INTERNAL FUNCTION Reads training data from a file descriptor.
 */
struct fann_train_data *fann_read_train_from_fd(FILE * file, const char *filename)
{
	unsigned int num_input, num_output, num_data, i, j;
	unsigned int line = 1;
	struct fann_train_data *data;

	if(fscanf(file, "%u %u %u\n", &num_data, &num_input, &num_output) != 3)
	{
		fann_error(NULL, FANN_E_CANT_READ_TD, filename, line);
		return NULL;
	}
	line++;

	data = fann_create_train(num_data, num_input, num_output);
	if(data == NULL)
	{
		return NULL;
	}

	for(i = 0; i != num_data; i++)
	{
		for(j = 0; j != num_input; j++)
		{
			if(fscanf(file, FANNSCANF " ", &data->input[i][j]) != 1)
			{
				fann_error(NULL, FANN_E_CANT_READ_TD, filename, line);
				fann_destroy_train(data);
				return NULL;
			}
		}
		line++;

		for(j = 0; j != num_output; j++)
		{
			if(fscanf(file, FANNSCANF " ", &data->output[i][j]) != 1)
			{
				fann_error(NULL, FANN_E_CANT_READ_TD, filename, line);
				fann_destroy_train(data);
				return NULL;
			}
		}
		line++;
	}
	return data;
}

/*
 * INTERNAL FUNCTION returns 0 if the desired error is reached and -1 if it is not reached
 */
int fann_desired_error_reached(struct fann *ann, float desired_error)
{
	switch (ann->train_stop_function)
	{
	case FANN_STOPFUNC_MSE:
		if(fann_get_MSE(ann) <= desired_error)
			return 0;
		break;
	case FANN_STOPFUNC_BIT:
		if(ann->num_bit_fail <= (unsigned int)desired_error)
			return 0;
		break;
	}
	return -1;
}

#ifndef FIXEDFANN
/*
 * Scale data in input vector before feed it to ann based on previously calculated parameters.
 */
FANN_EXTERNAL void FANN_API fann_scale_input( struct fann *ann, fann_type *input_vector )
{
	unsigned cur_neuron;
	if(ann->scale_mean_in == NULL)
	{
		fann_error( (struct fann_error *) ann, FANN_E_SCALE_NOT_PRESENT );
		return;
	}

	for( cur_neuron = 0; cur_neuron < ann->num_input; cur_neuron++ )
                if(ann->scale_deviation_in[ cur_neuron ] != 0.0)
                        input_vector[ cur_neuron ] =
                                (
                                        ( input_vector[ cur_neuron ] - ann->scale_mean_in[ cur_neuron ] )
                                        / ann->scale_deviation_in[ cur_neuron ]
                                        - ( (fann_type)-1.0 ) /* This is old_min */
                                )
                                * ann->scale_factor_in[ cur_neuron ]
                                + ann->scale_new_min_in[ cur_neuron ];
}

/*
 * Scale data in output vector before feed it to ann based on previously calculated parameters.
 */
FANN_EXTERNAL void FANN_API fann_scale_output( struct fann *ann, fann_type *output_vector )
{
	unsigned cur_neuron;
	if(ann->scale_mean_out == NULL)
	{
		fann_error( (struct fann_error *) ann, FANN_E_SCALE_NOT_PRESENT );
		return;
	}

	for( cur_neuron = 0; cur_neuron < ann->num_output; cur_neuron++ )
                if(ann->scale_deviation_out[ cur_neuron ] != 0.0)
                        output_vector[ cur_neuron ] =
                                (
                                        ( output_vector[ cur_neuron ] - ann->scale_mean_out[ cur_neuron ] )
                                        / ann->scale_deviation_out[ cur_neuron ]
                                        - ( (fann_type)-1.0 ) /* This is old_min */
                                )
                                * ann->scale_factor_out[ cur_neuron ]
                                + ann->scale_new_min_out[ cur_neuron ];
}

/*
 * Descale data in input vector after based on previously calculated parameters.
 */
FANN_EXTERNAL void FANN_API fann_descale_input( struct fann *ann, fann_type *input_vector )
{
	unsigned cur_neuron;
	if(ann->scale_mean_in == NULL)
	{
		fann_error( (struct fann_error *) ann, FANN_E_SCALE_NOT_PRESENT );
		return;
	}

	for( cur_neuron = 0; cur_neuron < ann->num_input; cur_neuron++ )
                if(ann->scale_deviation_in[ cur_neuron ] != 0.0)
                        input_vector[ cur_neuron ] =
                                (
                                        (
                                                input_vector[ cur_neuron ]
                                                - ann->scale_new_min_in[ cur_neuron ]
                                        )
                                        / ann->scale_factor_in[ cur_neuron ]
                                        + ( (fann_type)-1.0 ) /* This is old_min */
                                )
                                * ann->scale_deviation_in[ cur_neuron ]
                                + ann->scale_mean_in[ cur_neuron ];
}

/*
 * Descale data in output vector after get it from ann based on previously calculated parameters.
 */
FANN_EXTERNAL void FANN_API fann_descale_output( struct fann *ann, fann_type *output_vector )
{
	unsigned cur_neuron;
	if(ann->scale_mean_out == NULL)
	{
		fann_error( (struct fann_error *) ann, FANN_E_SCALE_NOT_PRESENT );
		return;
	}

	for( cur_neuron = 0; cur_neuron < ann->num_output; cur_neuron++ )
                if(ann->scale_deviation_out[ cur_neuron ] != 0.0)
                        output_vector[ cur_neuron ] =
                                (
                                        (
                                                output_vector[ cur_neuron ]
                                                - ann->scale_new_min_out[ cur_neuron ]
                                        )
                                        / ann->scale_factor_out[ cur_neuron ]
                                        + ( (fann_type)-1.0 ) /* This is old_min */
                                )
                                * ann->scale_deviation_out[ cur_neuron ]
                                + ann->scale_mean_out[ cur_neuron ];
}

/*
 * Scale input and output data based on previously calculated parameters.
 */
FANN_EXTERNAL void FANN_API fann_scale_train( struct fann *ann, struct fann_train_data *data )
{
	unsigned cur_sample;
	if(ann->scale_mean_in == NULL || ann->scale_mean_out == NULL)
	{
		fann_error( (struct fann_error *) ann, FANN_E_SCALE_NOT_PRESENT );
		return;
	}
	/* Check that we have good training data. */
	if(fann_check_input_output_sizes(ann, data) == -1)
		return;

	for( cur_sample = 0; cur_sample < data->num_data; cur_sample++ )
	{
		fann_scale_input( ann, data->input[ cur_sample ] );
		fann_scale_output( ann, data->output[ cur_sample ] );
	}
}

/*
 * Scale input and output data based on previously calculated parameters.
 */
FANN_EXTERNAL void FANN_API fann_descale_train( struct fann *ann, struct fann_train_data *data )
{
	unsigned cur_sample;
	if(ann->scale_mean_in == NULL || ann->scale_mean_out == NULL)
	{
		fann_error( (struct fann_error *) ann, FANN_E_SCALE_NOT_PRESENT );
		return;
	}
	/* Check that we have good training data. */
	if(fann_check_input_output_sizes(ann, data) == -1)
		return;

	for( cur_sample = 0; cur_sample < data->num_data; cur_sample++ )
	{
		fann_descale_input( ann, data->input[ cur_sample ] );
		fann_descale_output( ann, data->output[ cur_sample ] );
	}
}

#define SCALE_RESET( what, where, default_value )							\
	for( cur_neuron = 0; cur_neuron < ann->num_##where##put; cur_neuron++ )	\
		ann->what##_##where[ cur_neuron ] = ( default_value );

#define SCALE_SET_PARAM( where )																		\
	/* Calculate mean: sum(x)/length */																	\
	for( cur_neuron = 0; cur_neuron < ann->num_##where##put; cur_neuron++ )								\
		ann->scale_mean_##where[ cur_neuron ] = 0.0f;													\
	for( cur_neuron = 0; cur_neuron < ann->num_##where##put; cur_neuron++ )								\
		for( cur_sample = 0; cur_sample < data->num_data; cur_sample++ )								\
			ann->scale_mean_##where[ cur_neuron ] += (float)data->where##put[ cur_sample ][ cur_neuron ];\
	for( cur_neuron = 0; cur_neuron < ann->num_##where##put; cur_neuron++ )								\
		ann->scale_mean_##where[ cur_neuron ] /= (float)data->num_data;									\
	/* Calculate deviation: sqrt(sum((x-mean)^2)/length) */												\
	for( cur_neuron = 0; cur_neuron < ann->num_##where##put; cur_neuron++ )								\
		ann->scale_deviation_##where[ cur_neuron ] = 0.0f; 												\
	for( cur_neuron = 0; cur_neuron < ann->num_##where##put; cur_neuron++ )								\
		for( cur_sample = 0; cur_sample < data->num_data; cur_sample++ )								\
			ann->scale_deviation_##where[ cur_neuron ] += 												\
				/* Another local variable in macro? Oh no! */											\
				( 																						\
					(float)data->where##put[ cur_sample ][ cur_neuron ] 								\
					- ann->scale_mean_##where[ cur_neuron ] 											\
				) 																						\
				*																						\
				( 																						\
					(float)data->where##put[ cur_sample ][ cur_neuron ] 								\
					- ann->scale_mean_##where[ cur_neuron ] 											\
				); 																						\
	for( cur_neuron = 0; cur_neuron < ann->num_##where##put; cur_neuron++ )								\
		ann->scale_deviation_##where[ cur_neuron ] =													\
			sqrtf( ann->scale_deviation_##where[ cur_neuron ] / (float)data->num_data ); 			\
	/* Calculate factor: (new_max-new_min)/(old_max(1)-old_min(-1)) */									\
	/* Looks like we dont need whole array of factors? */												\
	for( cur_neuron = 0; cur_neuron < ann->num_##where##put; cur_neuron++ )								\
		ann->scale_factor_##where[ cur_neuron ] =														\
			( new_##where##put_max - new_##where##put_min )												\
			/																							\
			( 1.0f - ( -1.0f ) );																		\
	/* Copy new minimum. */																				\
	/* Looks like we dont need whole array of new minimums? */											\
	for( cur_neuron = 0; cur_neuron < ann->num_##where##put; cur_neuron++ )								\
		ann->scale_new_min_##where[ cur_neuron ] = new_##where##put_min;

FANN_EXTERNAL int FANN_API fann_set_input_scaling_params(
	struct fann *ann,
	const struct fann_train_data *data,
	float new_input_min,
	float new_input_max)
{
	unsigned cur_neuron, cur_sample;

	/* Check that we have good training data. */
	/* No need for if( !params || !ann ) */
	if(data->num_input != ann->num_input
	   || data->num_output != ann->num_output)
	{
		fann_error( (struct fann_error *) ann, FANN_E_TRAIN_DATA_MISMATCH );
		return -1;
	}

	if(ann->scale_mean_in == NULL)
		fann_allocate_scale(ann);

	if(ann->scale_mean_in == NULL)
		return -1;

	if( !data->num_data )
	{
		SCALE_RESET( scale_mean,		in,	0.0 )
		SCALE_RESET( scale_deviation,	in,	1.0 )
		SCALE_RESET( scale_new_min,		in,	-1.0 )
		SCALE_RESET( scale_factor,		in,	1.0 )
	}
	else
	{
		SCALE_SET_PARAM( in );
	}

	return 0;
}

FANN_EXTERNAL int FANN_API fann_set_output_scaling_params(
	struct fann *ann,
	const struct fann_train_data *data,
	float new_output_min,
	float new_output_max)
{
	unsigned cur_neuron, cur_sample;

	/* Check that we have good training data. */
	/* No need for if( !params || !ann ) */
	if(data->num_input != ann->num_input
	   || data->num_output != ann->num_output)
	{
		fann_error( (struct fann_error *) ann, FANN_E_TRAIN_DATA_MISMATCH );
		return -1;
	}

	if(ann->scale_mean_out == NULL)
		fann_allocate_scale(ann);

	if(ann->scale_mean_out == NULL)
		return -1;

	if( !data->num_data )
	{
		SCALE_RESET( scale_mean,		out,	0.0 )
		SCALE_RESET( scale_deviation,	out,	1.0 )
		SCALE_RESET( scale_new_min,		out,	-1.0 )
		SCALE_RESET( scale_factor,		out,	1.0 )
	}
	else
	{
		SCALE_SET_PARAM( out );
	}

	return 0;
}

/*
 * Calculate scaling parameters for future use based on training data.
 */
FANN_EXTERNAL int FANN_API fann_set_scaling_params(
	struct fann *ann,
	const struct fann_train_data *data,
	float new_input_min,
	float new_input_max,
	float new_output_min,
	float new_output_max)
{
	if(fann_set_input_scaling_params(ann, data, new_input_min, new_input_max) == 0)
		return fann_set_output_scaling_params(ann, data, new_output_min, new_output_max);
	else
		return -1;
}

/*
 * Clears scaling parameters.
 */
FANN_EXTERNAL int FANN_API fann_clear_scaling_params(struct fann *ann)
{
	unsigned cur_neuron;

	if(ann->scale_mean_out == NULL)
		fann_allocate_scale(ann);

	if(ann->scale_mean_out == NULL)
		return -1;

	SCALE_RESET( scale_mean,		in,	0.0 )
	SCALE_RESET( scale_deviation,	in,	1.0 )
	SCALE_RESET( scale_new_min,		in,	-1.0 )
	SCALE_RESET( scale_factor,		in,	1.0 )

	SCALE_RESET( scale_mean,		out,	0.0 )
	SCALE_RESET( scale_deviation,	out,	1.0 )
	SCALE_RESET( scale_new_min,		out,	-1.0 )
	SCALE_RESET( scale_factor,		out,	1.0 )

	return 0;
}

#endif

int fann_check_input_output_sizes(struct fann *ann, struct fann_train_data *data)
{
	if(ann->num_input != data->num_input)
    {
    	fann_error((struct fann_error *) ann, FANN_E_INPUT_NO_MATCH,
        	ann->num_input, data->num_input);
        return -1;
    }

	if(ann->num_output != data->num_output)
	{
		fann_error((struct fann_error *) ann, FANN_E_OUTPUT_NO_MATCH,
					ann->num_output, data->num_output);
		return -1;
	}

	return 0;
}


/*
file https://github.com/kaizhu256/fann/blob/v2021.2.21/src/fann_error.c
*/
/*
  Fast Artificial Neural Network Library (fann)
  Copyright (C) 2003-2016 Steffen Nissen (steffen.fann@gmail.com)

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <limits.h>

// #include "config.h"
// #include "fann.h"

#ifdef _MSC_VER
#define vsnprintf _vsnprintf
#define snprintf _snprintf
#endif

/* define path max if not defined */
#if defined(_WIN32) && !defined(__MINGW32__)
#define PATH_MAX _MAX_PATH
#endif
#ifndef PATH_MAX
#ifdef _POSIX_PATH_MAX
#define PATH_MAX _POSIX_PATH_MAX
#else
#define PATH_MAX 4096
#endif
#endif

FANN_EXTERNAL FILE * FANN_API fann_default_error_log = (FILE *)-1;

/* resets the last error number
 */
FANN_EXTERNAL void FANN_API fann_reset_errno(struct fann_error *errdat)
{
	errdat->errno_f = FANN_E_NO_ERROR;
}

/* resets the last errstr
 */
FANN_EXTERNAL void FANN_API fann_reset_errstr(struct fann_error *errdat)
{
	if(errdat->errstr != NULL)
		free(errdat->errstr);
	errdat->errstr = NULL;
}

/* returns the last error number
 */
FANN_EXTERNAL enum fann_errno_enum FANN_API fann_get_errno(struct fann_error *errdat)
{
	return errdat->errno_f;
}

/* returns the last errstr
 */
FANN_EXTERNAL char *FANN_API fann_get_errstr(struct fann_error *errdat)
{
	char *errstr = errdat->errstr;

	fann_reset_errno(errdat);
	fann_reset_errstr(errdat);

	return errstr;
}

/* change where errors are logged to
 */
FANN_EXTERNAL void FANN_API fann_set_error_log(struct fann_error *errdat, FILE * log_file)
{
	if(errdat == NULL)
		fann_default_error_log = log_file;
	else
		errdat->error_log = log_file;
}

/* prints the last error to stderr
 */
FANN_EXTERNAL void FANN_API fann_print_error(struct fann_error *errdat)
{
	if(errdat->errno_f != FANN_E_NO_ERROR && errdat->errstr != NULL)
	{
		fprintf(stderr, "FANN Error %d: %s", errdat->errno_f, errdat->errstr);
	}
}

/* INTERNAL FUNCTION
   Populate the error information
 */
void fann_error(struct fann_error *errdat, const enum fann_errno_enum errno_f, ...)
{
	va_list ap;
	size_t errstr_max = FANN_ERRSTR_MAX + PATH_MAX - 1;
	char errstr[FANN_ERRSTR_MAX + PATH_MAX];
	FILE * error_log = fann_default_error_log;

	if(errdat != NULL)
		errdat->errno_f = errno_f;

	va_start(ap, errno_f);
	switch (errno_f)
	{
	case FANN_E_NO_ERROR:
		return;
	case FANN_E_CANT_OPEN_CONFIG_R:
		vsnprintf(errstr, errstr_max, "Unable to open configuration file \"%s\" for reading.\n", ap);
		break;
	case FANN_E_CANT_OPEN_CONFIG_W:
		vsnprintf(errstr, errstr_max, "Unable to open configuration file \"%s\" for writing.\n", ap);
		break;
	case FANN_E_WRONG_CONFIG_VERSION:
		vsnprintf(errstr, errstr_max,
				 "Wrong version of configuration file, aborting read of configuration file \"%s\".\n",
				 ap);
		break;
	case FANN_E_CANT_READ_CONFIG:
		vsnprintf(errstr, errstr_max, "Error reading \"%s\" from configuration file \"%s\".\n", ap);
		break;
	case FANN_E_CANT_READ_NEURON:
		vsnprintf(errstr, errstr_max, "Error reading neuron info from configuration file \"%s\".\n", ap);
		break;
	case FANN_E_CANT_READ_CONNECTIONS:
		vsnprintf(errstr, errstr_max, "Error reading connections from configuration file \"%s\".\n", ap);
		break;
	case FANN_E_WRONG_NUM_CONNECTIONS:
		vsnprintf(errstr, errstr_max, "ERROR connections_so_far=%d, total_connections=%d\n", ap);
		break;
	case FANN_E_CANT_OPEN_TD_W:
		vsnprintf(errstr, errstr_max, "Unable to open train data file \"%s\" for writing.\n", ap);
		break;
	case FANN_E_CANT_OPEN_TD_R:
		vsnprintf(errstr, errstr_max, "Unable to open train data file \"%s\" for writing.\n", ap);
		break;
	case FANN_E_CANT_READ_TD:
		vsnprintf(errstr, errstr_max, "Error reading info from train data file \"%s\", line: %d.\n", ap);
		break;
	case FANN_E_CANT_ALLOCATE_MEM:
		strcpy(errstr, "Unable to allocate memory.\n");
		break;
	case FANN_E_CANT_TRAIN_ACTIVATION:
		strcpy(errstr, "Unable to train with the selected activation function.\n");
		break;
	case FANN_E_CANT_USE_ACTIVATION:
		strcpy(errstr, "Unable to use the selected activation function.\n");
		break;
	case FANN_E_TRAIN_DATA_MISMATCH:
		strcpy(errstr, "Training data must be of equivalent structure.\n");
		break;
	case FANN_E_CANT_USE_TRAIN_ALG:
		strcpy(errstr, "Unable to use the selected training algorithm.\n");
		break;
	case FANN_E_TRAIN_DATA_SUBSET:
		vsnprintf(errstr, errstr_max, "Subset from %d of length %d not valid in training set of length %d.\n", ap);
		break;
	case FANN_E_INDEX_OUT_OF_BOUND:
		vsnprintf(errstr, errstr_max, "Index %d is out of bound.\n", ap);
		break;
	case FANN_E_SCALE_NOT_PRESENT:
		strcpy(errstr, "Scaling parameters not present.\n");
		break;
    case FANN_E_INPUT_NO_MATCH:
		vsnprintf(errstr, errstr_max, "The number of input neurons in the ann (%d) and data (%d) don't match\n", ap);
    	break;
    case FANN_E_OUTPUT_NO_MATCH:
		vsnprintf(errstr, errstr_max, "The number of output neurons in the ann (%d) and data (%d) don't match\n", ap);
     	break;
	case FANN_E_WRONG_PARAMETERS_FOR_CREATE:
		strcpy(errstr, "The parameters for create_standard are wrong, either too few parameters provided or a negative/very high value provided.\n");
		break;
	}
	va_end(ap);

	if(errdat != NULL)
	{
		if(errdat->errstr == NULL)
		{
			errdat->errstr = (char*)malloc(strlen(errstr) + 1);
		}
		else if(strlen(errdat->errstr) < strlen(errstr))
		{
			errdat->errstr = (char*)realloc(errdat->errstr, strlen(errstr) + 1);
		}
		/* allocation failed */
		if(errdat->errstr == NULL)
		{
			fprintf(stderr, "Unable to allocate memory.\n");
			return;
		}
		strcpy(errdat->errstr, errstr);
		error_log = errdat->error_log;
	}

	if(error_log == (FILE *)-1) /* This is the default behavior and will give stderr */
	{
		fprintf(stderr, "FANN Error %d: %s", errno_f, errstr);
	}
	else if(error_log != NULL)
	{
		fprintf(error_log, "FANN Error %d: %s", errno_f, errstr);
	}
}

/* INTERNAL FUNCTION
   Initialize an error data strcuture
 */
void fann_init_error_data(struct fann_error *errdat)
{
	errdat->errstr = NULL;
	errdat->errno_f = FANN_E_NO_ERROR;
	errdat->error_log = fann_default_error_log;
}


/*
file https://github.com/kaizhu256/fann/blob/v2021.2.21/src/fann_cascade.c
*/
/*
  Fast Artificial Neural Network Library (fann)
  Copyright (C) 2003-2016 Steffen Nissen (steffen.fann@gmail.com)

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

// #include "config.h"
// #include "fann.h"
#include "string.h"

#ifndef FIXEDFANN

/* #define CASCADE_DEBUG */
/* #define CASCADE_DEBUG_FULL */

void fann_print_connections_raw(struct fann *ann)
{
	unsigned int i;

	for(i = 0; i < ann->total_connections_allocated; i++)
	{
		if(i == ann->total_connections)
		{
			printf("* ");
		}
		printf("%f ", ann->weights[i]);
	}
	printf("\n\n");
}

/* Cascade training directly on the training data.
   The connected_neurons pointers are not valid during training,
   but they will be again after training.
 */
FANN_EXTERNAL void FANN_API fann_cascadetrain_on_data(struct fann *ann, struct fann_train_data *data,
										unsigned int max_neurons,
										unsigned int neurons_between_reports,
										float desired_error)
{
	float error;
	unsigned int i;
	unsigned int total_epochs = 0;
	int desired_error_reached;

	if(neurons_between_reports && ann->callback == NULL)
	{
		printf("Max neurons %3d. Desired error: %.6f\n", max_neurons, desired_error);
	}

	for(i = 1; i <= max_neurons; i++)
	{
		/* train output neurons */
		total_epochs += fann_train_outputs(ann, data, desired_error);
		error = fann_get_MSE(ann);
		desired_error_reached = fann_desired_error_reached(ann, desired_error);

		/* print current error */
		if(neurons_between_reports &&
		   (i % neurons_between_reports == 0
			|| i == max_neurons || i == 1 || desired_error_reached == 0))
		{
			if(ann->callback == NULL)
			{
				printf
					("Neurons     %3d. Current error: %.6f. Total error:%8.4f. Epochs %5d. Bit fail %3d",
					 i-1, error, ann->MSE_value, total_epochs, ann->num_bit_fail);
				if((ann->last_layer-2) != ann->first_layer)
				{
					printf(". candidate steepness %.2f. function %s",
					   (ann->last_layer-2)->first_neuron->activation_steepness,
					   FANN_ACTIVATIONFUNC_NAMES[(ann->last_layer-2)->first_neuron->activation_function]);
				}
				printf("\n");
			}
			else if((*ann->callback) (ann, data, max_neurons,
				neurons_between_reports, desired_error, total_epochs) == -1)
			{
				/* you can break the training by returning -1 */
				break;
			}
		}

		if(desired_error_reached == 0)
			break;

		if(fann_initialize_candidates(ann) == -1)
		{
			/* Unable to initialize room for candidates */
			break;
		}

		/* train new candidates */
		total_epochs += fann_train_candidates(ann, data);

		/* this installs the best candidate */
		fann_install_candidate(ann);
	}

	/* Train outputs one last time but without any desired error */
	total_epochs += fann_train_outputs(ann, data, 0.0);

	if(neurons_between_reports && ann->callback == NULL)
	{
		printf("Train outputs    Current error: %.6f. Epochs %6d\n", fann_get_MSE(ann),
			   total_epochs);
	}

	/* Set pointers in connected_neurons
	 * This is ONLY done in the end of cascade training,
	 * since there is no need for them during training.
	 */
	fann_set_shortcut_connections(ann);
}

FANN_EXTERNAL void FANN_API fann_cascadetrain_on_file(struct fann *ann, const char *filename,
													  unsigned int max_neurons,
													  unsigned int neurons_between_reports,
													  float desired_error)
{
	struct fann_train_data *data = fann_read_train_from_file(filename);

	if(data == NULL)
	{
		return;
	}
	fann_cascadetrain_on_data(ann, data, max_neurons, neurons_between_reports, desired_error);
	fann_destroy_train(data);
}

int fann_train_outputs(struct fann *ann, struct fann_train_data *data, float desired_error)
{
	float error, initial_error, error_improvement;
	float target_improvement = 0.0;
	float backslide_improvement = -1.0e20f;
	unsigned int i;
	unsigned int max_epochs = ann->cascade_max_out_epochs;
	unsigned int min_epochs = ann->cascade_min_out_epochs;
	unsigned int stagnation = max_epochs;

	/* TODO should perhaps not clear all arrays */
	fann_clear_train_arrays(ann);

	/* run an initial epoch to set the initital error */
	initial_error = fann_train_outputs_epoch(ann, data);

	if(fann_desired_error_reached(ann, desired_error) == 0)
		return 1;

	for(i = 1; i < max_epochs; i++)
	{
		error = fann_train_outputs_epoch(ann, data);

		/*printf("Epoch %6d. Current error: %.6f. Bit fail %d.\n", i, error, ann->num_bit_fail); */

		if(fann_desired_error_reached(ann, desired_error) == 0)
		{
#ifdef CASCADE_DEBUG
			printf("Error %f < %f\n", error, desired_error);
#endif
			return i + 1;
		}

		/* Improvement since start of train */
		error_improvement = initial_error - error;

		/* After any significant change, set a new goal and
		 * allow a new quota of epochs to reach it */

		if((target_improvement >= 0 &&
			(error_improvement > target_improvement || error_improvement < backslide_improvement)) ||
		(target_improvement < 0 &&
			(error_improvement < target_improvement || error_improvement > backslide_improvement)))
		{
			/*printf("error_improvement=%f, target_improvement=%f, backslide_improvement=%f, stagnation=%d\n", error_improvement, target_improvement, backslide_improvement, stagnation); */

			target_improvement = error_improvement * (1.0f + ann->cascade_output_change_fraction);
			backslide_improvement = error_improvement * (1.0f - ann->cascade_output_change_fraction);
			stagnation = i + ann->cascade_output_stagnation_epochs;
		}

		/* No improvement in allotted period, so quit */
		if(i >= stagnation && i >= min_epochs)
		{
			return i + 1;
		}
	}

	return max_epochs;
}

float fann_train_outputs_epoch(struct fann *ann, struct fann_train_data *data)
{
	unsigned int i;

	fann_reset_MSE(ann);

	for(i = 0; i < data->num_data; i++)
	{
		fann_run(ann, data->input[i]);
		fann_compute_MSE(ann, data->output[i]);
		fann_update_slopes_batch(ann, ann->last_layer - 1, ann->last_layer - 1);
	}

	switch (ann->training_algorithm)
	{
		case FANN_TRAIN_RPROP:
			fann_update_weights_irpropm(ann, (ann->last_layer - 1)->first_neuron->first_con,
										ann->total_connections);
			break;
		case FANN_TRAIN_SARPROP:
			fann_update_weights_sarprop(ann, ann->sarprop_epoch, (ann->last_layer - 1)->first_neuron->first_con,
										ann->total_connections);
			++(ann->sarprop_epoch);
			break;
		case FANN_TRAIN_QUICKPROP:
			fann_update_weights_quickprop(ann, data->num_data,
										  (ann->last_layer - 1)->first_neuron->first_con,
										  ann->total_connections);
			break;
		case FANN_TRAIN_BATCH:
		case FANN_TRAIN_INCREMENTAL:
			fann_error((struct fann_error *) ann, FANN_E_CANT_USE_TRAIN_ALG);
	}

	return fann_get_MSE(ann);
}

int fann_reallocate_connections(struct fann *ann, unsigned int total_connections)
{
	/* The connections are allocated, but the pointers inside are
	 * first moved in the end of the cascade training session.
	 */

#ifdef CASCADE_DEBUG
	printf("realloc from %d to %d\n", ann->total_connections_allocated, total_connections);
#endif
	ann->connections =
		(struct fann_neuron **) realloc(ann->connections,
										total_connections * sizeof(struct fann_neuron *));
	if(ann->connections == NULL)
	{
		fann_error((struct fann_error *) ann, FANN_E_CANT_ALLOCATE_MEM);
		return -1;
	}

	ann->weights = (fann_type *) realloc(ann->weights, total_connections * sizeof(fann_type));
	if(ann->weights == NULL)
	{
		fann_error((struct fann_error *) ann, FANN_E_CANT_ALLOCATE_MEM);
		return -1;
	}

	ann->train_slopes =
		(fann_type *) realloc(ann->train_slopes, total_connections * sizeof(fann_type));
	if(ann->train_slopes == NULL)
	{
		fann_error((struct fann_error *) ann, FANN_E_CANT_ALLOCATE_MEM);
		return -1;
	}

	ann->prev_steps = (fann_type *) realloc(ann->prev_steps, total_connections * sizeof(fann_type));
	if(ann->prev_steps == NULL)
	{
		fann_error((struct fann_error *) ann, FANN_E_CANT_ALLOCATE_MEM);
		return -1;
	}

	ann->prev_train_slopes =
		(fann_type *) realloc(ann->prev_train_slopes, total_connections * sizeof(fann_type));
	if(ann->prev_train_slopes == NULL)
	{
		fann_error((struct fann_error *) ann, FANN_E_CANT_ALLOCATE_MEM);
		return -1;
	}

	ann->total_connections_allocated = total_connections;

	return 0;
}

int fann_reallocate_neurons(struct fann *ann, unsigned int total_neurons)
{
	struct fann_layer *layer_it;
	struct fann_neuron *neurons;
	unsigned int num_neurons = 0;
	unsigned int num_neurons_so_far = 0;

	neurons =
		(struct fann_neuron *) realloc(ann->first_layer->first_neuron,
									   total_neurons * sizeof(struct fann_neuron));
	ann->total_neurons_allocated = total_neurons;

	if(neurons == NULL)
	{
		fann_error((struct fann_error *) ann, FANN_E_CANT_ALLOCATE_MEM);
		return -1;
	}

	/* Also allocate room for more train_errors */
	ann->train_errors = (fann_type *) realloc(ann->train_errors, total_neurons * sizeof(fann_type));
	if(ann->train_errors == NULL)
	{
		fann_error((struct fann_error *) ann, FANN_E_CANT_ALLOCATE_MEM);
		return -1;
	}

	if(neurons != ann->first_layer->first_neuron)
	{
		/* Then the memory has moved, also move the pointers */

#ifdef CASCADE_DEBUG_FULL
		printf("Moving neuron pointers\n");
#endif

		/* Move pointers from layers to neurons */
		for(layer_it = ann->first_layer; layer_it != ann->last_layer; layer_it++)
		{
			num_neurons = (unsigned int)(layer_it->last_neuron - layer_it->first_neuron);
			layer_it->first_neuron = neurons + num_neurons_so_far;
			layer_it->last_neuron = layer_it->first_neuron + num_neurons;
			num_neurons_so_far += num_neurons;
		}
	}

	return 0;
}

void initialize_candidate_weights(struct fann *ann, unsigned int first_con, unsigned int last_con, float scale_factor)
{
	fann_type prev_step;
	unsigned int i = 0;
	unsigned int bias_weight = (unsigned int)(first_con + (ann->first_layer->last_neuron - ann->first_layer->first_neuron) - 1);

	if(ann->training_algorithm == FANN_TRAIN_RPROP)
		prev_step = ann->rprop_delta_zero;
	else
		prev_step = 0;

	for(i = first_con; i < last_con; i++)
	{
		if(i == bias_weight)
			ann->weights[i] = fann_rand(-scale_factor, scale_factor);
		else
			ann->weights[i] = fann_rand(0,scale_factor);

		ann->train_slopes[i] = 0;
		ann->prev_steps[i] = prev_step;
		ann->prev_train_slopes[i] = 0;
	}
}

int fann_initialize_candidates(struct fann *ann)
{
	/* The candidates are allocated after the normal neurons and connections,
	 * but there is an empty place between the real neurons and the candidate neurons,
	 * so that it will be possible to make room when the chosen candidate are copied in
	 * on the desired place.
	 */
	unsigned int neurons_to_allocate, connections_to_allocate;
	unsigned int num_candidates = fann_get_cascade_num_candidates(ann);
	unsigned int num_neurons = ann->total_neurons + num_candidates + 1;
	unsigned int num_hidden_neurons = ann->total_neurons - ann->num_input - ann->num_output;
	unsigned int candidate_connections_in = ann->total_neurons - ann->num_output;
	unsigned int candidate_connections_out = ann->num_output;

	/* the number of connections going into a and out of a candidate is
	 * ann->total_neurons */
	unsigned int num_connections =
		ann->total_connections + (ann->total_neurons * (num_candidates + 1));
	unsigned int first_candidate_connection = ann->total_connections + ann->total_neurons;
	unsigned int first_candidate_neuron = ann->total_neurons + 1;
	unsigned int connection_it, i, j, k, candidate_index;
	struct fann_neuron *neurons;
	float scale_factor;

	/* First make sure that there is enough room, and if not then allocate a
	 * bit more so that we do not need to allocate more room each time.
	 */
	if(num_neurons > ann->total_neurons_allocated)
	{
		/* Then we need to allocate more neurons
		 * Allocate half as many neurons as already exist (at least ten)
		 */
		neurons_to_allocate = num_neurons + num_neurons / 2;
		if(neurons_to_allocate < num_neurons + 10)
		{
			neurons_to_allocate = num_neurons + 10;
		}

		if(fann_reallocate_neurons(ann, neurons_to_allocate) == -1)
		{
			return -1;
		}
	}

	if(num_connections > ann->total_connections_allocated)
	{
		/* Then we need to allocate more connections
		 * Allocate half as many connections as already exist
		 * (at least enough for ten neurons)
		 */
		connections_to_allocate = num_connections + num_connections / 2;
		if(connections_to_allocate < num_connections + ann->total_neurons * 10)
		{
			connections_to_allocate = num_connections + ann->total_neurons * 10;
		}

		if(fann_reallocate_connections(ann, connections_to_allocate) == -1)
		{
			return -1;
		}
	}

	/* Some code to do semi Widrow + Nguyen initialization */
	scale_factor = (float) (2.0 * pow(0.7f * (float)num_hidden_neurons, 1.0f / (float) ann->num_input));
	if(scale_factor > 8)
		scale_factor = 8;
	else if(scale_factor < 0.5)
		scale_factor = 0.5;

	/* Set the neurons.
	 */
	connection_it = first_candidate_connection;
	neurons = ann->first_layer->first_neuron;
	candidate_index = first_candidate_neuron;

	for(i = 0; i < ann->cascade_activation_functions_count; i++)
	{
		for(j = 0; j < ann->cascade_activation_steepnesses_count; j++)
		{
			for(k = 0; k < ann->cascade_num_candidate_groups; k++)
			{
				/* TODO candidates should actually be created both in
				 * the last layer before the output layer, and in a new layer.
				 */
				neurons[candidate_index].value = 0;
				neurons[candidate_index].sum = 0;

				neurons[candidate_index].activation_function =
					ann->cascade_activation_functions[i];
				neurons[candidate_index].activation_steepness =
					ann->cascade_activation_steepnesses[j];

				neurons[candidate_index].first_con = connection_it;
				connection_it += candidate_connections_in;
				neurons[candidate_index].last_con = connection_it;
				/* We have no specific pointers to the output weights, but they are
				 * available after last_con */
				connection_it += candidate_connections_out;
				ann->train_errors[candidate_index] = 0;
				initialize_candidate_weights(ann, neurons[candidate_index].first_con, neurons[candidate_index].last_con+candidate_connections_out, scale_factor);
				candidate_index++;
			}
		}
	}


	/* Now randomize the weights and zero out the arrays that needs zeroing out.
	 */
	 /*
#ifdef CASCADE_DEBUG_FULL
	printf("random cand weight [%d ... %d]\n", first_candidate_connection, num_connections - 1);
#endif

	for(i = first_candidate_connection; i < num_connections; i++)
	{

		//ann->weights[i] = fann_random_weight();
		ann->weights[i] = fann_rand(-2.0,2.0);
		ann->train_slopes[i] = 0;
		ann->prev_steps[i] = 0;
		ann->prev_train_slopes[i] = initial_slope;
	}
	*/

	return 0;
}

int fann_train_candidates(struct fann *ann, struct fann_train_data *data)
{
	fann_type best_cand_score = 0.0;
	fann_type target_cand_score = 0.0;
	fann_type backslide_cand_score = -1.0e20f;
	unsigned int i;
	unsigned int max_epochs = ann->cascade_max_cand_epochs;
	unsigned int min_epochs = ann->cascade_min_cand_epochs;
	unsigned int stagnation = max_epochs;

	if(ann->cascade_candidate_scores == NULL)
	{
		ann->cascade_candidate_scores =
			(fann_type *) malloc(fann_get_cascade_num_candidates(ann) * sizeof(fann_type));
		if(ann->cascade_candidate_scores == NULL)
		{
			fann_error((struct fann_error *) ann, FANN_E_CANT_ALLOCATE_MEM);
			return 0;
		}
	}

	for(i = 0; i < max_epochs; i++)
	{
		best_cand_score = fann_train_candidates_epoch(ann, data);

		if(best_cand_score / ann->MSE_value > ann->cascade_candidate_limit)
		{
#ifdef CASCADE_DEBUG
			printf("above candidate limit %f/%f > %f", best_cand_score, ann->MSE_value,
				   ann->cascade_candidate_limit);
#endif
			return i + 1;
		}

		if((best_cand_score > target_cand_score) || (best_cand_score < backslide_cand_score))
		{
#ifdef CASCADE_DEBUG_FULL
			printf("Best candidate score %f, real score: %f\n", ann->MSE_value - best_cand_score,
				   best_cand_score);
			/* printf("best_cand_score=%f, target_cand_score=%f, backslide_cand_score=%f, stagnation=%d\n", best_cand_score, target_cand_score, backslide_cand_score, stagnation); */
#endif

			target_cand_score = best_cand_score * (1.0f + ann->cascade_candidate_change_fraction);
			backslide_cand_score = best_cand_score * (1.0f - ann->cascade_candidate_change_fraction);
			stagnation = i + ann->cascade_candidate_stagnation_epochs;
		}

		/* No improvement in allotted period, so quit */
		if(i >= stagnation && i >= min_epochs)
		{
#ifdef CASCADE_DEBUG
			printf("Stagnation with %d epochs, best candidate score %f, real score: %f\n", i + 1,
				   ann->MSE_value - best_cand_score, best_cand_score);
#endif
			return i + 1;
		}
	}

#ifdef CASCADE_DEBUG
	printf("Max epochs %d reached, best candidate score %f, real score: %f\n", max_epochs,
		   ann->MSE_value - best_cand_score, best_cand_score);
#endif
	return max_epochs;
}

void fann_update_candidate_slopes(struct fann *ann)
{
	struct fann_neuron *neurons = ann->first_layer->first_neuron;
	struct fann_neuron *first_cand = neurons + ann->total_neurons + 1;
	struct fann_neuron *last_cand = first_cand + fann_get_cascade_num_candidates(ann);
	struct fann_neuron *cand_it;
	unsigned int i, j, num_connections;
	unsigned int num_output = ann->num_output;
	fann_type max_sum, cand_sum, activation, derived, error_value, diff, cand_score;
	fann_type *weights, *cand_out_weights, *cand_slopes, *cand_out_slopes;
	fann_type *output_train_errors = ann->train_errors + (ann->total_neurons - ann->num_output);

	for(cand_it = first_cand; cand_it < last_cand; cand_it++)
	{
		cand_score = ann->cascade_candidate_scores[cand_it - first_cand];
		error_value = 0.0;

		/* code more or less stolen from fann_run to fast forward pass
		 */
		cand_sum = 0.0;
		num_connections = cand_it->last_con - cand_it->first_con;
		weights = ann->weights + cand_it->first_con;

		/* unrolled loop start */
		i = num_connections & 3;	/* same as modulo 4 */
		switch (i)
		{
			case 3:
				cand_sum += weights[2] * neurons[2].value;
			case 2:
				cand_sum += weights[1] * neurons[1].value;
			case 1:
				cand_sum += weights[0] * neurons[0].value;
			case 0:
				break;
		}

		for(; i != num_connections; i += 4)
		{
			cand_sum +=
				weights[i] * neurons[i].value +
				weights[i + 1] * neurons[i + 1].value +
				weights[i + 2] * neurons[i + 2].value + weights[i + 3] * neurons[i + 3].value;
		}
		/*
		 * for(i = 0; i < num_connections; i++){
		 * cand_sum += weights[i] * neurons[i].value;
		 * }
		 */
		/* unrolled loop end */

		max_sum = 150/cand_it->activation_steepness;
		if(cand_sum > max_sum)
			cand_sum = max_sum;
		else if(cand_sum < -max_sum)
			cand_sum = -max_sum;

		activation =
			fann_activation(ann, cand_it->activation_function, cand_it->activation_steepness,
							cand_sum);
		/* printf("%f = sigmoid(%f);\n", activation, cand_sum); */

		cand_it->sum = cand_sum;
		cand_it->value = activation;

		derived = fann_activation_derived(cand_it->activation_function,
										  cand_it->activation_steepness, activation, cand_sum);

		/* The output weights is located right after the input weights in
		 * the weight array.
		 */
		cand_out_weights = weights + num_connections;

		cand_out_slopes = ann->train_slopes + cand_it->first_con + num_connections;
		for(j = 0; j < num_output; j++)
		{
			diff = (activation * cand_out_weights[j]) - output_train_errors[j];
#ifdef CASCADE_DEBUG_FULL
			/* printf("diff = %f = (%f * %f) - %f;\n", diff, activation, cand_out_weights[j], output_train_errors[j]); */
#endif
			cand_out_slopes[j] -= 2.0f * diff * activation;
#ifdef CASCADE_DEBUG_FULL
			/* printf("cand_out_slopes[%d] <= %f += %f * %f;\n", j, cand_out_slopes[j], diff, activation); */
#endif
			error_value += diff * cand_out_weights[j];
			cand_score -= (diff * diff);
#ifdef CASCADE_DEBUG_FULL
			/* printf("cand_score[%d][%d] = %f -= (%f * %f)\n", cand_it - first_cand, j, cand_score, diff, diff); */

			printf("cand[%d]: error=%f, activation=%f, diff=%f, slope=%f\n", cand_it - first_cand,
				   output_train_errors[j], (activation * cand_out_weights[j]), diff,
				   -2.0 * diff * activation);
#endif
		}

		ann->cascade_candidate_scores[cand_it - first_cand] = cand_score;
		error_value *= derived;

		cand_slopes = ann->train_slopes + cand_it->first_con;
		for(i = 0; i < num_connections; i++)
		{
			cand_slopes[i] -= error_value * neurons[i].value;
		}
	}
}

void fann_update_candidate_weights(struct fann *ann, unsigned int num_data)
{
	struct fann_neuron *first_cand = (ann->last_layer - 1)->last_neuron + 1;	/* there is an empty neuron between the actual neurons and the candidate neuron */
	struct fann_neuron *last_cand = first_cand + fann_get_cascade_num_candidates(ann) - 1;

	switch (ann->training_algorithm)
	{
		case FANN_TRAIN_RPROP:
			fann_update_weights_irpropm(ann, first_cand->first_con,
										last_cand->last_con + ann->num_output);
			break;
		case FANN_TRAIN_SARPROP:
			/* TODO: increase epoch? */
			fann_update_weights_sarprop(ann, ann->sarprop_epoch, first_cand->first_con,
										last_cand->last_con + ann->num_output);
			break;
		case FANN_TRAIN_QUICKPROP:
			fann_update_weights_quickprop(ann, num_data, first_cand->first_con,
										  last_cand->last_con + ann->num_output);
			break;
		case FANN_TRAIN_BATCH:
		case FANN_TRAIN_INCREMENTAL:
			fann_error((struct fann_error *) ann, FANN_E_CANT_USE_TRAIN_ALG);
			break;
	}
}

fann_type fann_train_candidates_epoch(struct fann *ann, struct fann_train_data *data)
{
	unsigned int i, j;
	unsigned int best_candidate;
	fann_type best_score;
	unsigned int num_cand = fann_get_cascade_num_candidates(ann);
	fann_type *output_train_errors = ann->train_errors + (ann->total_neurons - ann->num_output);
	struct fann_neuron *output_neurons = (ann->last_layer - 1)->first_neuron;

	for(i = 0; i < num_cand; i++)
	{
		/* The ann->MSE_value is actually the sum squared error */
		ann->cascade_candidate_scores[i] = ann->MSE_value;
	}
	/*printf("start score: %f\n", ann->MSE_value); */

	for(i = 0; i < data->num_data; i++)
	{
		fann_run(ann, data->input[i]);

		for(j = 0; j < ann->num_output; j++)
		{
			/* TODO only debug, but the error is in opposite direction, this might be usefull info */
			/*          if(output_train_errors[j] != (ann->output[j] - data->output[i][j])){
			 * printf("difference in calculated error at %f != %f; %f = %f - %f;\n", output_train_errors[j], (ann->output[j] - data->output[i][j]), output_train_errors[j], ann->output[j], data->output[i][j]);
			 * } */

			/*
			 * output_train_errors[j] = (data->output[i][j] - ann->output[j])/2;
			 * output_train_errors[j] = ann->output[j] - data->output[i][j];
			 */

			output_train_errors[j] = (data->output[i][j] - ann->output[j]);

			switch (output_neurons[j].activation_function)
			{
				case FANN_LINEAR_PIECE_SYMMETRIC:
				case FANN_SIGMOID_SYMMETRIC:
				case FANN_SIGMOID_SYMMETRIC_STEPWISE:
				case FANN_THRESHOLD_SYMMETRIC:
				case FANN_ELLIOT_SYMMETRIC:
				case FANN_GAUSSIAN_SYMMETRIC:
				case FANN_SIN_SYMMETRIC:
				case FANN_COS_SYMMETRIC:
					output_train_errors[j] /= 2.0;
					break;
				case FANN_LINEAR:
				case FANN_THRESHOLD:
				case FANN_SIGMOID:
				case FANN_SIGMOID_STEPWISE:
				case FANN_GAUSSIAN:
				case FANN_GAUSSIAN_STEPWISE:
				case FANN_ELLIOT:
				case FANN_LINEAR_PIECE:
				case FANN_SIN:
				case FANN_COS:
					break;
			}
		}

		fann_update_candidate_slopes(ann);
	}

	fann_update_candidate_weights(ann, data->num_data);

	/* find the best candidate score */
	best_candidate = 0;
	best_score = ann->cascade_candidate_scores[best_candidate];
	for(i = 1; i < num_cand; i++)
	{
		/*struct fann_neuron *cand = ann->first_layer->first_neuron + ann->total_neurons + 1 + i;
		 * printf("candidate[%d] = activation: %s, steepness: %f, score: %f\n",
		 * i, FANN_ACTIVATIONFUNC_NAMES[cand->activation_function],
		 * cand->activation_steepness, ann->cascade_candidate_scores[i]); */

		if(ann->cascade_candidate_scores[i] > best_score)
		{
			best_candidate = i;
			best_score = ann->cascade_candidate_scores[best_candidate];
		}
	}

	ann->cascade_best_candidate = ann->total_neurons + best_candidate + 1;
#ifdef CASCADE_DEBUG
	printf("Best candidate[%d]: with score %f, real score: %f\n", best_candidate,
		   ann->MSE_value - best_score, best_score);
#endif

	return best_score;
}

/* add a layer at the position pointed to by *layer */
struct fann_layer *fann_add_layer(struct fann *ann, struct fann_layer *layer)
{
	int layer_pos = (int)(layer - ann->first_layer);
	int num_layers = (int)(ann->last_layer - ann->first_layer + 1);
	int i;

	/* allocate the layer */
	struct fann_layer *layers =
		(struct fann_layer *) realloc(ann->first_layer, num_layers * sizeof(struct fann_layer));
	if(layers == NULL)
	{
		fann_error((struct fann_error *) ann, FANN_E_CANT_ALLOCATE_MEM);
		return NULL;
	}

	/* copy layers so that the free space is at the right location */
	for(i = num_layers - 1; i >= layer_pos; i--)
	{
		layers[i] = layers[i - 1];
	}

	/* the newly allocated layer is empty */
	layers[layer_pos].first_neuron = layers[layer_pos + 1].first_neuron;
	layers[layer_pos].last_neuron = layers[layer_pos + 1].first_neuron;

	/* Set the ann pointers correctly */
	ann->first_layer = layers;
	ann->last_layer = layers + num_layers;

#ifdef CASCADE_DEBUG_FULL
	printf("add layer at pos %d\n", layer_pos);
#endif

	return layers + layer_pos;
}

void fann_set_shortcut_connections(struct fann *ann)
{
	struct fann_layer *layer_it;
	struct fann_neuron *neuron_it, **neuron_pointers, *neurons;
	unsigned int num_connections = 0, i;

	neuron_pointers = ann->connections;
	neurons = ann->first_layer->first_neuron;

	for(layer_it = ann->first_layer + 1; layer_it != ann->last_layer; layer_it++)
	{
		for(neuron_it = layer_it->first_neuron; neuron_it != layer_it->last_neuron; neuron_it++)
		{

			neuron_pointers += num_connections;
			num_connections = neuron_it->last_con - neuron_it->first_con;

			for(i = 0; i != num_connections; i++)
			{
				neuron_pointers[i] = neurons + i;
			}
		}
	}
}

void fann_add_candidate_neuron(struct fann *ann, struct fann_layer *layer)
{
	unsigned int num_connections_in = (unsigned int)(layer->first_neuron - ann->first_layer->first_neuron);
	unsigned int num_connections_out = (unsigned int)((ann->last_layer - 1)->last_neuron - (layer + 1)->first_neuron);
	unsigned int num_connections_move = num_connections_out + num_connections_in;

	unsigned int candidate_con, candidate_output_weight;
	int i;

	struct fann_layer *layer_it;
	struct fann_neuron *neuron_it, *neuron_place, *candidate;

	/* We know that there is enough room for the new neuron
	 * (the candidates are in the same arrays), so move
	 * the last neurons to make room for this neuron.
	 */

	/* first move the pointers to neurons in the layer structs */
	for(layer_it = ann->last_layer - 1; layer_it != layer; layer_it--)
	{
#ifdef CASCADE_DEBUG_FULL
		printf("move neuron pointers in layer %d, first(%d -> %d), last(%d -> %d)\n",
			   layer_it - ann->first_layer,
			   layer_it->first_neuron - ann->first_layer->first_neuron,
			   layer_it->first_neuron - ann->first_layer->first_neuron + 1,
			   layer_it->last_neuron - ann->first_layer->first_neuron,
			   layer_it->last_neuron - ann->first_layer->first_neuron + 1);
#endif
		layer_it->first_neuron++;
		layer_it->last_neuron++;
	}

	/* also move the last neuron in the layer that needs the neuron added */
	layer->last_neuron++;

	/* this is the place that should hold the new neuron */
	neuron_place = layer->last_neuron - 1;

#ifdef CASCADE_DEBUG_FULL
	printf("num_connections_in=%d, num_connections_out=%d\n", num_connections_in,
		   num_connections_out);
#endif

	candidate = ann->first_layer->first_neuron + ann->cascade_best_candidate;

	/* the output weights for the candidates are located after the input weights */
	candidate_output_weight = candidate->last_con;

	/* move the actual output neurons and the indexes to the connection arrays */
	for(neuron_it = (ann->last_layer - 1)->last_neuron - 1; neuron_it != neuron_place; neuron_it--)
	{
#ifdef CASCADE_DEBUG_FULL
		printf("move neuron %d -> %d\n", neuron_it - ann->first_layer->first_neuron - 1,
			   neuron_it - ann->first_layer->first_neuron);
#endif
		*neuron_it = *(neuron_it - 1);

		/* move the weights */
#ifdef CASCADE_DEBUG_FULL
		printf("move weight[%d ... %d] -> weight[%d ... %d]\n", neuron_it->first_con,
			   neuron_it->last_con - 1, neuron_it->first_con + num_connections_move - 1,
			   neuron_it->last_con + num_connections_move - 2);
#endif
		for(i = neuron_it->last_con - 1; i >= (int)neuron_it->first_con; i--)
		{
#ifdef CASCADE_DEBUG_FULL
			printf("move weight[%d] = weight[%d]\n", i + num_connections_move - 1, i);
#endif
			ann->weights[i + num_connections_move - 1] = ann->weights[i];
		}

		/* move the indexes to weights */
		neuron_it->last_con += num_connections_move;
		num_connections_move--;
		neuron_it->first_con += num_connections_move;

		/* set the new weight to the newly allocated neuron */
		ann->weights[neuron_it->last_con - 1] =
			(ann->weights[candidate_output_weight]) * ann->cascade_weight_multiplier;
		candidate_output_weight++;
	}

	/* Now inititalize the actual neuron */
	neuron_place->value = 0;
	neuron_place->sum = 0;
	neuron_place->activation_function = candidate->activation_function;
	neuron_place->activation_steepness = candidate->activation_steepness;
	neuron_place->last_con = (neuron_place + 1)->first_con;
	neuron_place->first_con = neuron_place->last_con - num_connections_in;
#ifdef CASCADE_DEBUG_FULL
	printf("neuron[%d] = weights[%d ... %d] activation: %s, steepness: %f\n",
		   neuron_place - ann->first_layer->first_neuron, neuron_place->first_con,
		   neuron_place->last_con - 1, FANN_ACTIVATIONFUNC_NAMES[neuron_place->activation_function],
		   neuron_place->activation_steepness);/* TODO remove */
#endif

	candidate_con = candidate->first_con;
	/* initialize the input weights at random */
#ifdef CASCADE_DEBUG_FULL
	printf("move cand weights[%d ... %d] -> [%d ... %d]\n", candidate_con,
		   candidate_con + num_connections_in - 1, neuron_place->first_con,
		   neuron_place->last_con - 1);
#endif

	for(i = 0; i < (int)num_connections_in; i++)
	{
		ann->weights[i + neuron_place->first_con] = ann->weights[i + candidate_con];
#ifdef CASCADE_DEBUG_FULL
		printf("move weights[%d] -> weights[%d] (%f)\n", i + candidate_con,
			   i + neuron_place->first_con, ann->weights[i + neuron_place->first_con]);
#endif
	}

	/* Change some of main variables */
	ann->total_neurons++;
	ann->total_connections += num_connections_in + num_connections_out;

	return;
}

void fann_install_candidate(struct fann *ann)
{
	struct fann_layer *layer;

	layer = fann_add_layer(ann, ann->last_layer - 1);
	fann_add_candidate_neuron(ann, layer);
	return;
}

#endif /* FIXEDFANN */

FANN_EXTERNAL unsigned int FANN_API fann_get_cascade_num_candidates(struct fann *ann)
{
	return ann->cascade_activation_functions_count *
		ann->cascade_activation_steepnesses_count *
		ann->cascade_num_candidate_groups;
}

FANN_GET_SET(float, cascade_output_change_fraction)
FANN_GET_SET(unsigned int, cascade_output_stagnation_epochs)
FANN_GET_SET(float, cascade_candidate_change_fraction)
FANN_GET_SET(unsigned int, cascade_candidate_stagnation_epochs)
FANN_GET_SET(unsigned int, cascade_num_candidate_groups)
FANN_GET_SET(fann_type, cascade_weight_multiplier)
FANN_GET_SET(fann_type, cascade_candidate_limit)
FANN_GET_SET(unsigned int, cascade_max_out_epochs)
FANN_GET_SET(unsigned int, cascade_max_cand_epochs)
FANN_GET_SET(unsigned int, cascade_min_out_epochs)
FANN_GET_SET(unsigned int, cascade_min_cand_epochs)

FANN_GET(unsigned int, cascade_activation_functions_count)
FANN_GET(enum fann_activationfunc_enum *, cascade_activation_functions)

FANN_EXTERNAL void FANN_API fann_set_cascade_activation_functions(struct fann *ann,
														 enum fann_activationfunc_enum *
														 cascade_activation_functions,
														 unsigned int
														 cascade_activation_functions_count)
{
	if(ann->cascade_activation_functions_count != cascade_activation_functions_count)
	{
		ann->cascade_activation_functions_count = cascade_activation_functions_count;

		/* reallocate mem */
		ann->cascade_activation_functions =
			(enum fann_activationfunc_enum *)realloc(ann->cascade_activation_functions,
			ann->cascade_activation_functions_count * sizeof(enum fann_activationfunc_enum));
		if(ann->cascade_activation_functions == NULL)
		{
			fann_error((struct fann_error*)ann, FANN_E_CANT_ALLOCATE_MEM);
			return;
		}
	}

	memmove(ann->cascade_activation_functions, cascade_activation_functions,
		ann->cascade_activation_functions_count * sizeof(enum fann_activationfunc_enum));
}

FANN_GET(unsigned int, cascade_activation_steepnesses_count)
FANN_GET(fann_type *, cascade_activation_steepnesses)

FANN_EXTERNAL void FANN_API fann_set_cascade_activation_steepnesses(struct fann *ann,
														   fann_type *
														   cascade_activation_steepnesses,
														   unsigned int
														   cascade_activation_steepnesses_count)
{
	if(ann->cascade_activation_steepnesses_count != cascade_activation_steepnesses_count)
	{
		ann->cascade_activation_steepnesses_count = cascade_activation_steepnesses_count;

		/* reallocate mem */
		ann->cascade_activation_steepnesses =
			(fann_type *)realloc(ann->cascade_activation_steepnesses,
			ann->cascade_activation_steepnesses_count * sizeof(fann_type));
		if(ann->cascade_activation_steepnesses == NULL)
		{
			fann_error((struct fann_error*)ann, FANN_E_CANT_ALLOCATE_MEM);
			return;
		}
	}

	memmove(ann->cascade_activation_steepnesses, cascade_activation_steepnesses,
		ann->cascade_activation_steepnesses_count * sizeof(fann_type));
}


/*
file https://github.com/kaizhu256/fann/blob/v2021.2.21/src/parallel_fann.c
*/
/*
 * parallel_FANN.c
 *     Author: Alessandro Pietro Bardelli
 */
#ifndef DISABLE_PARALLEL_FANN
#include <omp.h>
// #include "parallel_fann.h"
// #include "config.h"
// #include "fann.h"

FANN_EXTERNAL float FANN_API fann_train_epoch_batch_parallel(struct fann *ann, struct fann_train_data *data, const unsigned int threadnumb)
{
	/*vector<struct fann *> ann_vect(threadnumb);*/
	struct fann** ann_vect= (struct fann**) malloc(threadnumb * sizeof(struct fann*));
	int i=0,j=0;
	fann_reset_MSE(ann);

	//generate copies of the ann
	omp_set_dynamic(0);
	omp_set_num_threads(threadnumb);
	#pragma omp parallel private(j)
	{

		#pragma omp for schedule(static)
		for(i=0; i<(int)threadnumb; i++)
		{
			ann_vect[i]=fann_copy(ann);
		}

    //parallel computing of the updates

        #pragma omp for schedule(static)
		for(i = 0; i < (int)data->num_data; i++)
		{
			j=omp_get_thread_num();
			fann_run(ann_vect[j], data->input[i]);
			fann_compute_MSE(ann_vect[j], data->output[i]);
			fann_backpropagate_MSE(ann_vect[j]);
			fann_update_slopes_batch(ann_vect[j], ann_vect[j]->first_layer + 1, ann_vect[j]->last_layer - 1);
		}
	}

    //parallel update of the weights
	{
		const unsigned int num_data=data->num_data;
		const unsigned int first_weight=0;
		const unsigned int past_end=ann->total_connections;
		fann_type *weights = ann->weights;
		const fann_type epsilon = ann->learning_rate / num_data;
		omp_set_dynamic(0);
		omp_set_num_threads(threadnumb);
		#pragma omp parallel
		{
			#pragma omp for schedule(static)
				for(i=first_weight; i < (int)past_end; i++)
				{
					fann_type temp_slopes=0.0;
					unsigned int k;
					fann_type *train_slopes;
					for(k=0;k<threadnumb;++k)
					{
						train_slopes=ann_vect[k]->train_slopes;
						temp_slopes+= train_slopes[i];
						train_slopes[i]=0.0;
					}
					weights[i] += temp_slopes*epsilon;
				}
			}
	}
	//merge of MSEs
	for(i=0;i<(int)threadnumb;++i)
	{
		ann->MSE_value+= ann_vect[i]->MSE_value;
		ann->num_MSE+=ann_vect[i]->num_MSE;
		fann_destroy(ann_vect[i]);
	}
	free(ann_vect);
	return fann_get_MSE(ann);
}


FANN_EXTERNAL float FANN_API fann_train_epoch_irpropm_parallel(struct fann *ann, struct fann_train_data *data, const unsigned int threadnumb)
{
	struct fann** ann_vect= (struct fann**) malloc(threadnumb * sizeof(struct fann*));
	int i=0,j=0;

	if(ann->prev_train_slopes == NULL)
	{
		fann_clear_train_arrays(ann);
	}

	//#define THREADNUM 1
	fann_reset_MSE(ann);

	/*vector<struct fann *> ann_vect(threadnumb);*/

	//generate copies of the ann
	omp_set_dynamic(0);
	omp_set_num_threads(threadnumb);
	#pragma omp parallel private(j)
	{

		#pragma omp for schedule(static)
		for(i=0; i<(int)threadnumb; i++)
		{
			ann_vect[i]=fann_copy(ann);
		}

    //parallel computing of the updates


        #pragma omp for schedule(static)
		for(i = 0; i < (int)data->num_data; i++)
		{
			j=omp_get_thread_num();
			fann_run(ann_vect[j], data->input[i]);
			fann_compute_MSE(ann_vect[j], data->output[i]);
			fann_backpropagate_MSE(ann_vect[j]);
			fann_update_slopes_batch(ann_vect[j], ann_vect[j]->first_layer + 1, ann_vect[j]->last_layer - 1);
		}
	}

	{
    	fann_type *weights = ann->weights;
    	fann_type *prev_steps = ann->prev_steps;
    	fann_type *prev_train_slopes = ann->prev_train_slopes;

    	fann_type next_step;

    	const float increase_factor = ann->rprop_increase_factor;	//1.2;
    	const float decrease_factor = ann->rprop_decrease_factor;	//0.5;
    	const float delta_min = ann->rprop_delta_min;	//0.0;
    	const float delta_max = ann->rprop_delta_max;	//50.0;
		const unsigned int first_weight=0;
		const unsigned int past_end=ann->total_connections;

		omp_set_dynamic(0);
		omp_set_num_threads(threadnumb);
		#pragma omp parallel private(next_step)
		{
			#pragma omp for schedule(static)
				for(i=first_weight; i < (int)past_end; i++)
				{
					fann_type prev_slope, same_sign;
		    		const fann_type prev_step = fann_max(prev_steps[i], (fann_type) 0.0001);	// prev_step may not be zero because then the training will stop

		    		fann_type temp_slopes=0.0;
					unsigned int k;
					fann_type *train_slopes;
					for(k=0;k<threadnumb;++k)
					{
						train_slopes=ann_vect[k]->train_slopes;
						temp_slopes+= train_slopes[i];
						train_slopes[i]=0.0;
					}

		    		prev_slope = prev_train_slopes[i];

		    		same_sign = prev_slope * temp_slopes;

		    		if(same_sign >= 0.0)
		    			next_step = fann_min(prev_step * increase_factor, delta_max);
		    		else
		    		{
		    			next_step = fann_max(prev_step * decrease_factor, delta_min);
		    			temp_slopes = 0;
		    		}

		    		if(temp_slopes < 0)
		    		{
		    			weights[i] -= next_step;
		    			if(weights[i] < -1500)
		    				weights[i] = -1500;
		    		}
		    		else
		    		{
		    			weights[i] += next_step;
		    			if(weights[i] > 1500)
		    				weights[i] = 1500;
		    		}

		    		// update global data arrays
		    		prev_steps[i] = next_step;
		    		prev_train_slopes[i] = temp_slopes;

				}
			}
	}

	//merge of MSEs
	for(i=0;i<(int)threadnumb;++i)
	{
		ann->MSE_value+= ann_vect[i]->MSE_value;
		ann->num_MSE+=ann_vect[i]->num_MSE;
		fann_destroy(ann_vect[i]);
	}
	free(ann_vect);
	return fann_get_MSE(ann);
}


FANN_EXTERNAL float FANN_API fann_train_epoch_quickprop_parallel(struct fann *ann, struct fann_train_data *data, const unsigned int threadnumb)
{
	struct fann** ann_vect= (struct fann**) malloc(threadnumb * sizeof(struct fann*));
	int i=0,j=0;

	if(ann->prev_train_slopes == NULL)
	{
		fann_clear_train_arrays(ann);
	}

	//#define THREADNUM 1
	fann_reset_MSE(ann);

	/*vector<struct fann *> ann_vect(threadnumb);*/

	//generate copies of the ann
	omp_set_dynamic(0);
	omp_set_num_threads(threadnumb);
	#pragma omp parallel private(j)
	{

		#pragma omp for schedule(static)
		for(i=0; i<(int)threadnumb; i++)
		{
			ann_vect[i]=fann_copy(ann);
		}

    //parallel computing of the updates

        #pragma omp for schedule(static)
		for(i = 0; i < (int)data->num_data; i++)
		{
			j=omp_get_thread_num();
			fann_run(ann_vect[j], data->input[i]);
			fann_compute_MSE(ann_vect[j], data->output[i]);
			fann_backpropagate_MSE(ann_vect[j]);
			fann_update_slopes_batch(ann_vect[j], ann_vect[j]->first_layer + 1, ann_vect[j]->last_layer - 1);
		}
	}

    {
    	fann_type *weights = ann->weights;
    	fann_type *prev_steps = ann->prev_steps;
    	fann_type *prev_train_slopes = ann->prev_train_slopes;
		const unsigned int first_weight=0;
		const unsigned int past_end=ann->total_connections;

    	fann_type w=0.0, next_step;

    	const float epsilon = ann->learning_rate / data->num_data;
    	const float decay = ann->quickprop_decay;	/*-0.0001;*/
    	const float mu = ann->quickprop_mu;	/*1.75; */
    	const float shrink_factor = (float) (mu / (1.0 + mu));

		omp_set_dynamic(0);
		omp_set_num_threads(threadnumb);
		#pragma omp parallel private(w, next_step)
		{
			#pragma omp for schedule(static)
				for(i=first_weight; i < (int)past_end; i++)
				{
					fann_type temp_slopes=0.0;
					unsigned int k;
					fann_type *train_slopes;
					fann_type prev_step, prev_slope;

					w = weights[i];
					for(k=0;k<threadnumb;++k)
					{
						train_slopes=ann_vect[k]->train_slopes;
						temp_slopes+= train_slopes[i];
						train_slopes[i]=0.0;
					}
					temp_slopes+= decay * w;

					prev_step = prev_steps[i];
					prev_slope = prev_train_slopes[i];

					next_step = 0.0;


					/* The step must always be in direction opposite to the slope. */
					if(prev_step > 0.001)
					{
						/* If last step was positive...  */
						if(temp_slopes > 0.0) /*  Add in linear term if current slope is still positive. */
							next_step += epsilon * temp_slopes;

						/*If current slope is close to or larger than prev slope...  */
						if(temp_slopes > (shrink_factor * prev_slope))
							next_step += mu * prev_step;	/* Take maximum size negative step. */
						else
							next_step += prev_step * temp_slopes / (prev_slope - temp_slopes);	/* Else, use quadratic estimate. */
					}
					else if(prev_step < -0.001)
					{
						/* If last step was negative...  */
						if(temp_slopes < 0.0) /*  Add in linear term if current slope is still negative. */
							next_step += epsilon * temp_slopes;

						/* If current slope is close to or more neg than prev slope... */
						if(temp_slopes < (shrink_factor * prev_slope))
							next_step += mu * prev_step;	/* Take maximum size negative step. */
						else
							next_step += prev_step * temp_slopes / (prev_slope - temp_slopes);	/* Else, use quadratic estimate. */
					}
					else /* Last step was zero, so use only linear term. */
						next_step += epsilon * temp_slopes;

					/* update global data arrays */
					prev_steps[i] = next_step;
					prev_train_slopes[i] = temp_slopes;

					w += next_step;

					if(w > 1500)
						weights[i] = 1500;
					else if(w < -1500)
						weights[i] = -1500;
					else
						weights[i] = w;
				}
		}
	}
	//merge of MSEs
	for(i=0;i<(int)threadnumb;++i)
	{
		ann->MSE_value+= ann_vect[i]->MSE_value;
		ann->num_MSE+=ann_vect[i]->num_MSE;
		fann_destroy(ann_vect[i]);
	}
	free(ann_vect);
	return fann_get_MSE(ann);
}


FANN_EXTERNAL float FANN_API fann_train_epoch_sarprop_parallel(struct fann *ann, struct fann_train_data *data, const unsigned int threadnumb)
{
	struct fann** ann_vect= (struct fann**) malloc(threadnumb * sizeof(struct fann*));
	int i=0,j=0;

	if(ann->prev_train_slopes == NULL)
	{
		fann_clear_train_arrays(ann);
	}

	//#define THREADNUM 1
	fann_reset_MSE(ann);

	/*vector<struct fann *> ann_vect(threadnumb);*/

	//generate copies of the ann
	omp_set_dynamic(0);
	omp_set_num_threads(threadnumb);
	#pragma omp parallel private(j)
	{

		#pragma omp for schedule(static)
		for(i=0; i<(int)threadnumb; i++)
		{
			ann_vect[i]=fann_copy(ann);
		}

    //parallel computing of the updates

        #pragma omp for schedule(static)
		for(i = 0; i < (int)data->num_data; i++)
		{
			j=omp_get_thread_num();
			fann_run(ann_vect[j], data->input[i]);
			fann_compute_MSE(ann_vect[j], data->output[i]);
			fann_backpropagate_MSE(ann_vect[j]);
			fann_update_slopes_batch(ann_vect[j], ann_vect[j]->first_layer + 1, ann_vect[j]->last_layer - 1);
		}
	}

    {
    	fann_type *weights = ann->weights;
    	fann_type *prev_steps = ann->prev_steps;
    	fann_type *prev_train_slopes = ann->prev_train_slopes;
		const unsigned int first_weight=0;
		const unsigned int past_end=ann->total_connections;
		const unsigned int epoch=ann->sarprop_epoch;

    	fann_type next_step;

    	/* These should be set from variables */
    	const float increase_factor = ann->rprop_increase_factor;	/*1.2; */
    	const float decrease_factor = ann->rprop_decrease_factor;	/*0.5; */
    	/* TODO: why is delta_min 0.0 in iRprop? SARPROP uses 1x10^-6 (Braun and Riedmiller, 1993) */
    	const float delta_min = 0.000001f;
    	const float delta_max = ann->rprop_delta_max;	/*50.0; */
    	const float weight_decay_shift = ann->sarprop_weight_decay_shift; /* ld 0.01 = -6.644 */
    	const float step_error_threshold_factor = ann->sarprop_step_error_threshold_factor; /* 0.1 */
    	const float step_error_shift = ann->sarprop_step_error_shift; /* ld 3 = 1.585 */
    	const float T = ann->sarprop_temperature;
		float MSE, RMSE;


    	//merge of MSEs
    	for(i=0;i<(int)threadnumb;++i)
    	{
    		ann->MSE_value+= ann_vect[i]->MSE_value;
    		ann->num_MSE+=ann_vect[i]->num_MSE;
    	}

    	MSE = fann_get_MSE(ann);
    	RMSE = sqrtf(MSE);

    	/* for all weights; TODO: are biases included? */
		omp_set_dynamic(0);
		omp_set_num_threads(threadnumb);
		#pragma omp parallel private(next_step)
		{
			#pragma omp for schedule(static)
				for(i=first_weight; i < (int)past_end; i++)
				{
					/* TODO: confirm whether 1x10^-6 == delta_min is really better */
					const fann_type prev_step  = fann_max(prev_steps[i], (fann_type) 0.000001);	/* prev_step may not be zero because then the training will stop */

					/* calculate SARPROP slope; TODO: better as new error function? (see SARPROP paper)*/
					fann_type prev_slope, same_sign;
					fann_type temp_slopes=0.0;
					unsigned int k;
					fann_type *train_slopes;
					for(k=0;k<threadnumb;++k)
					{
						train_slopes=ann_vect[k]->train_slopes;
						temp_slopes+= train_slopes[i];
						train_slopes[i]=0.0;
					}
					temp_slopes= -temp_slopes - weights[i] * (fann_type)fann_exp2(-T * epoch + weight_decay_shift);

					next_step=0.0;

					/* TODO: is prev_train_slopes[i] 0.0 in the beginning? */
					prev_slope = prev_train_slopes[i];

					same_sign = prev_slope * temp_slopes;

					if(same_sign > 0.0)
					{
						next_step = fann_min(prev_step * increase_factor, delta_max);
						/* TODO: are the signs inverted? see differences between SARPROP paper and iRprop */
						if (temp_slopes < 0.0)
							weights[i] += next_step;
						else
							weights[i] -= next_step;
					}
					else if(same_sign < 0.0)
					{
						#ifndef RAND_MAX
						#define	RAND_MAX	0x7fffffff
						#endif
						if(prev_step < step_error_threshold_factor * MSE)
							next_step = prev_step * decrease_factor + (float)rand() / RAND_MAX * RMSE * (fann_type)fann_exp2(-T * epoch + step_error_shift);
						else
							next_step = fann_max(prev_step * decrease_factor, delta_min);

						temp_slopes = 0.0;
					}
					else
					{
						if(temp_slopes < 0.0)
							weights[i] += prev_step;
						else
							weights[i] -= prev_step;
					}

					/* update global data arrays */
					prev_steps[i] = next_step;
					prev_train_slopes[i] = temp_slopes;

				}
		}
    }

	++(ann->sarprop_epoch);

	//already computed before
	/*//merge of MSEs
	for(i=0;i<threadnumb;++i)
	{
		ann->MSE_value+= ann_vect[i]->MSE_value;
		ann->num_MSE+=ann_vect[i]->num_MSE;
	}*/
	//destroy the copies of the ann
	for(i=0; i<(int)threadnumb; i++)
	{
		fann_destroy(ann_vect[i]);
	}
	free(ann_vect);
	return fann_get_MSE(ann);
}

FANN_EXTERNAL float FANN_API fann_train_epoch_incremental_mod(struct fann *ann, struct fann_train_data *data)
{
	unsigned int i;

	fann_reset_MSE(ann);

	for(i = 0; i != data->num_data; i++)
	{
		fann_train(ann, data->input[i], data->output[i]);
	}

	return fann_get_MSE(ann);
}

#endif /* DISABLE_PARALLEL_FANN */


/*
file none
*/
/*jslint-enable*/
