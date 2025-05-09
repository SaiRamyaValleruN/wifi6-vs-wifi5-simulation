.. include:: replace.txt
.. highlight:: cpp

.. heading hierarchy:
   ------------- Chapter
   ************* Section (#.#)
   ============= Subsection (#.#.#)
   ############# Paragraph (no number)

Logging
-------

The |ns3| logging facility can be used to monitor or debug the progress
of simulation programs.  Logging output can be enabled by program statements
in your ``main()`` program or by the use of the ``NS_LOG`` environment variable.

Logging statements are not compiled into ``optimized`` builds of |ns3|.  To use
logging, one must use the ``default`` or ``debug`` build profiles of |ns3|.

The project makes no guarantee about whether logging output will remain
the same over time.  Users are cautioned against building simulation output
frameworks on top of logging code, as the output and the way the output
is enabled may change over time.

Overview
********

|ns3| logging statements are typically used to log various program
execution events, such as the occurrence of simulation events or the
use of a particular function.

For example, this code snippet is from ``TcpSocketBase::EnterCwr()`` and informs the user that
the model is reducing the congestion window and changing state::

  NS_LOG_INFO("Enter CWR recovery mode; set cwnd to " << m_tcb->m_cWnd << ", ssthresh to "
                                                      << m_tcb->m_ssThresh << ", recover to "
                                                      << m_recover);

If logging has been enabled for the ``Ipv4L3Protocol`` component at a severity
of ``INFO`` or above (see below about log severity), the statement
will be printed out; otherwise, it will be suppressed.

The logging implementation is enabled in ``debug`` and ``default``
builds, but disabled in all other build profiles,
so that it does not impact the execution speed of more optimized profiles.

You can try the example program `log-example.cc` in `src/core/example`
with various values for the `NS_LOG` environment variable to see the
effect of the options discussed below.

Enabling Output
***************

There are two ways that users typically control log output.  The
first is by setting the ``NS_LOG`` environment variable; e.g.:

.. sourcecode:: bash

   $ NS_LOG="*" ./ns3 run first

will run the ``first`` tutorial program with all logging output.  (The
specifics of the ``NS_LOG`` format will be discussed below.)

This can be made more granular by selecting individual components:

.. sourcecode:: bash

   $ NS_LOG="Ipv4L3Protocol" ./ns3 run first

The output can be further tailored with prefix options.

The second way to enable logging is to use explicit statements in your
program, such as in the ``first`` tutorial program::

   int
   main(int argc, char *argv[])
   {
     LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
     LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);
     ...

(The meaning of ``LOG_LEVEL_INFO``, and other possible values,
will be discussed below.)

``NS_LOG`` Syntax
*****************

The ``NS_LOG`` environment variable contains a list of log components
and options.  Log components are separated by \`:' characters:

.. sourcecode:: bash

   $ NS_LOG="<log-component>:<log-component>..."

Options for each log component are given as flags after
each log component:

.. sourcecode:: bash

   $ NS_LOG="<log-component>=<option>|<option>...:<log-component>..."

Options control the severity and level for that component,
and whether optional information should be included, such as the
simulation time, simulation node, function name, and the symbolic severity.

Log Components
==============

Generally a log component refers to a single source code ``.cc`` file,
and encompasses the entire file.

Some helpers have special methods to enable the logging of all components
in a module, spanning different compilation units, but logically grouped
together, such as the |ns3| wifi code::

   WifiHelper wifiHelper;
   wifiHelper.EnableLogComponents();

The ``NS_LOG`` log component wildcard \`*' will enable all components.

To see what log components are defined, any of these will work:

.. sourcecode:: bash

   $ NS_LOG="print-list" ./ns3 run ...

   $ NS_LOG="foo"  # a token not matching any log-component

The first form will print the name and enabled flags for all log components
which are linked in; try it with ``scratch-simulator``.
The second form prints all registered log components,
then exit with an error.


Severity and Level Options
==========================

Individual messages belong to a single "severity class," set by the macro
creating the message.  In the example above,
``NS_LOG_INFO(..)`` creates the message in the ``LOG_INFO`` severity class.

The following severity classes are defined as ``enum`` constants:

================  =========================================================
Severity Class    Meaning
================  =========================================================
``LOG_NONE``      The default, no logging
``LOG_ERROR``     Serious error messages only
``LOG_WARN``      Warning messages
``LOG_INFO``      Info about the model changing state
``LOG_FUNCTION``  Function tracing
``LOG_LOGIC``     For tracing key decision points or branches in a function
``LOG_DEBUG``     For use in debugging
================  =========================================================

Typically one wants to see messages at a given severity class *and higher*.
This is done by defining inclusive logging "levels":

======================  ===========================================
Level                   Meaning
======================  ===========================================
``LOG_LEVEL_ERROR``     Only ``LOG_ERROR`` severity class messages.
``LOG_LEVEL_WARN``      ``LOG_WARN`` and above.
``LOG_LEVEL_INFO``      ``LOG_INFO`` and above.
``LOG_LEVEL_FUNCTION``  ``LOG_FUNCTION`` and above.
``LOG_LEVEL_LOGIC``     ``LOG_LOGIC`` and above.
``LOG_LEVEL_DEBUG``     ``LOG_DEBUG`` and above.
``LOG_LEVEL_ALL``       All severity classes.
``LOG_ALL``             Synonym for ``LOG_LEVEL_ALL``
======================  ===========================================

The severity class and level options can be given in the ``NS_LOG``
environment variable by these tokens:

============  =================
Class         Level
============  =================
``error``     ``level_error``
``warn``      ``level_warn``
``info``      ``level_info``
``function``  ``level_function``
``logic``     ``level_logic``
``debug``     ``level_debug``
..            | ``level_all``
              | ``all``
              | ``*``
============  =================

Using a severity class token enables log messages at that severity only.
For example, ``NS_LOG="*=warn"`` won't output messages with severity ``error``.
``NS_LOG="*=level_debug"`` will output messages at severity levels
``debug`` and above.

Severity classes and levels can be combined with the \`|' operator:
``NS_LOG="*=level_warn|debug"`` will output messages at severity levels
``error``, ``warn`` and ``debug``, but not ``info``, ``function``, or ``logic``.

The ``NS_LOG`` severity level wildcard \`*' and ``all``
are synonyms for ``level_all``.

For log components merely mentioned in ``NS_LOG``

.. sourcecode:: bash

   $ NS_LOG="<log-component>:..."

the default severity is ``LOG_LEVEL_ALL``.


Prefix Options
==============

A number of prefixes can help identify
where and when a message originated, and at what severity.

The available prefix options (as ``enum`` constants) are

======================  ===========================================
Prefix Symbol           Meaning
======================  ===========================================
``LOG_PREFIX_FUNC``     Prefix the name of the calling function.
``LOG_PREFIX_TIME``     Prefix the simulation time.
``LOG_PREFIX_NODE``     Prefix the node id.
``LOG_PREFIX_LEVEL``    Prefix the severity level.
``LOG_PREFIX_ALL``      Enable all prefixes.
======================  ===========================================

The prefix options are described briefly below.

The options can be given in the ``NS_LOG``
environment variable by these tokens:

================  =========
Token             Alternate
================  =========
``prefix_func``   ``func``
``prefix_time``   ``time``
``prefix_node``   ``node``
``prefix_level``  ``level``
``prefix_all``    | ``all``
                  | ``*``
================  =========

For log components merely mentioned in ``NS_LOG``

.. sourcecode:: bash

   $ NS_LOG="<log-component>:..."

the default prefix options are ``LOG_PREFIX_ALL``.

Severity Prefix
###############

The severity class of a message can be included with the options
``prefix_level`` or ``level``.  For example, this value of ``NS_LOG``
enables logging for all log components (\`*') and all severity
classes (``=all``), and prefixes the message with the severity
class (``|prefix_level``).

.. sourcecode:: bash

   $ NS_LOG="*=all|prefix_level" ./ns3 run scratch-simulator
   Scratch Simulator
   [ERROR] error message
   [WARN] warn message
   [INFO] info message
   [FUNCT] function message
   [LOGIC] logic message
   [DEBUG] debug message

Time Prefix
###########

The simulation time can be included with the options
``prefix_time`` or ``time``.  This prints the simulation time in seconds.

Node Prefix
###########

The simulation node id can be included with the options
``prefix_node`` or ``node``.

Function Prefix
###############

The name of the calling function can be included with the options
``prefix_func`` or ``func``.


``NS_LOG`` Wildcards
====================

The log component wildcard \`*' will enable all components.  To
enable all components at a specific severity level
use ``*=<severity>``.

The severity level option wildcard \`*' is a synonym for ``all``.
This must occur before any \`|' characters separating options.
To enable all severity classes, use ``<log-component>=*``,
or ``<log-component>=*|<options>``.

The option wildcard \`*' or token ``all`` enables all prefix options,
but must occur *after* a \`|' character.  To enable a specific
severity class or level, and all prefixes, use
``<log-component>=<severity>|*``.

The combined option wildcard ``**`` enables all severities and all prefixes;
for example, ``<log-component>=**``.

The uber-wildcard ``***`` enables all severities and all prefixes
for all log components.  These are all equivalent:

.. sourcecode:: bash

   $ NS_LOG="***" ...      $ NS_LOG="*=all|*" ...        $ NS_LOG="*=*|all" ...
   $ NS_LOG="*=**" ...     $ NS_LOG="*=level_all|*" ...  $ NS_LOG="*=*|prefix_all" ...
   $ NS_LOG="*=*|*" ...

Be advised:  even the trivial ``scratch-simulator`` produces over
46K lines of output with ``NS_LOG="***"``!


How to add logging to your code
*******************************

Adding logging to your code is very simple:

1. Invoke the ``NS_LOG_COMPONENT_DEFINE(...);`` macro
   inside of ``namespace ns3``.

  Create a unique string identifier (usually based on the name of the file
  and/or class defined within the file) and register it with a macro call
  such as follows:

  ::

    namespace ns3 {

    NS_LOG_COMPONENT_DEFINE("Ipv4L3Protocol");
    ...

  This registers ``Ipv4L3Protocol`` as a log component.

  (The macro was carefully written to permit inclusion either within or
  outside of namespace ``ns3``, and usage will vary across the codebase, but
  the original intent was to register this *outside* of namespace ``ns3``
  at file global scope.)

2. Add logging statements (macro calls) to your functions and function bodies.

In case you want to add logging statements to the methods of your template class
(which are defined in an header file):

1. Invoke the ``NS_LOG_TEMPLATE_DECLARE;`` macro in the private section of
   your class declaration. For instance:

  ::

    template <typename Item>
    class Queue : public QueueBase
    {
    ...
    private:
      std::list<Ptr<Item>> m_packets;           //!< the items in the queue
      NS_LOG_TEMPLATE_DECLARE;                  //!< the log component
    };

  This requires you to perform these steps for all the subclasses of your class.

2. Invoke the ``NS_LOG_TEMPLATE_DEFINE(...);`` macro in the constructor of
   your class by providing the name of a log component registered by calling
   the ``NS_LOG_COMPONENT_DEFINE(...);`` macro in some module. For instance:

  ::

    template <typename Item>
    Queue<Item>::Queue()
      : NS_LOG_TEMPLATE_DEFINE("Queue")
    {
    }

3. Add logging statements (macro calls) to the methods of your class.

In case you want to add logging statements to a static member template
(which is defined in an header file):

1. Invoke the ``NS_LOG_STATIC_TEMPLATE_DEFINE (...);`` macro in your static
   method by providing the name of a log component registered by calling
   the ``NS_LOG_COMPONENT_DEFINE(...);`` macro in some module. For instance:

  ::

    template <typename Item>
    void
    NetDeviceQueue::PacketEnqueued(Ptr<Queue<Item>> queue,
                                   Ptr<NetDeviceQueueInterface> ndqi,
                                   uint8_t txq, Ptr<const Item> item)
    {

      NS_LOG_STATIC_TEMPLATE_DEFINE("NetDeviceQueueInterface");
    ...

2. Add logging statements (macro calls) to your static method.

Logging Macros
==============

  The logging macros and associated severity levels are

  ================  ==========================
  Severity Class    Macro
  ================  ==========================
  ``LOG_NONE``      (none needed)
  ``LOG_ERROR``     ``NS_LOG_ERROR(...);``
  ``LOG_WARN``      ``NS_LOG_WARN(...);``
  ``LOG_INFO``      ``NS_LOG_INFO(...);``
  ``LOG_FUNCTION``  ``NS_LOG_FUNCTION(...);``
  ``LOG_LOGIC``     ``NS_LOG_LOGIC(...);``
  ``LOG_DEBUG``     ``NS_LOG_DEBUG(...);``
  ================  ==========================

  The macros function as output streamers, so anything you can send to
  ``std::cout``, joined by ``<<`` operators, is allowed::

    void MyClass::Check(int value, char * item)
    {
      NS_LOG_FUNCTION(this << arg << item);
      if (arg > 10)
        {
          NS_LOG_ERROR("encountered bad value " << value <<
                       " while checking " << name << "!");
        }
      ...
    }

  Note that ``NS_LOG_FUNCTION`` automatically inserts a \`\ :literal:`,\ `'
  (comma-space) separator between each of its arguments.
  This simplifies logging of function arguments;
  just concatenate them with ``<<`` as in the example above.

Unconditional Logging
=====================

As a convenience, the ``NS_LOG_UNCOND(...);`` macro will always log its
arguments, even if the associated log-component is not enabled at any
severity.  This macro does not use any of the prefix options.  Recall
that logging is only enabled in ``debug``, ``default`` and ``relwithdebinfo``
builds, so this macro will only produce output in the same builds.

The |ns3| model libraries do not use the ``NS_LOG_UNCOND(...)`` macro;
it is provided for users for assistance with debugging.  The most common
use case is if a user is debugging some code and doesn't want to enable
all log macros in a file because it would produce too much output. Instead,
selected log statements can temporarily be changed from, e.g., ``NS_LOG_DEBUG(...)`` to ``NS_LOG_UNCOND(...)`` for the duration of the debugging session.
Don't forget to undo such a change before committing a bug fix.

Logging Blocks of Code
======================

Sometimes the calculations of variables to be included in a log statement is
involved and may not easily fit into a single ``NS_LOG`` statement.
The variables can raise compilation warnings when the logging is disabled, and
in any case, any unnecessary operations should be avoided.  For this situation,
authors can include a block of code in logging using the
`#ifdef NS3_LOG_ENABLE`` macro, as demonstrated in the sample below.

::

  #ifdef NS3_LOG_ENABLE
  {
      // Expensive calculation that is only used for logging
      int total = 0;
      for (const auto& v : myVector)
      {
          total += complexCalculation(v);
      }

      NS_LOG_DEBUG("Total: " << total);
  }
  #endif // NS3_LOG_ENABLE

Please remember to check that any such complicated logging statements have no
side effects that would cause different simulation execution in optimized
builds.

Guidelines
==========

* Start every significant class method with ``NS_LOG_FUNCTION(this << args...);``
  This enables easy function call tracing.

  * Exception 1:  don't log operators or explicit copy constructors,
    since these will cause infinite recursion and stack overflow.

  * Exception 2:  For simple methods such as getters, avoid function
    logging because it tends to overload the logging output.

  * For methods without arguments use the same form:
    ``NS_LOG_FUNCTION(this);``

  * For static functions:

    * With arguments use ``NS_LOG_FUNCTION(...);`` as normal.
    * Without arguments use ``NS_LOG_FUNCTION_NOARGS();``

* Use ``NS_LOG_ERROR`` for serious error conditions that probably
  invalidate the simulation execution.  Note that in |ns3|, we typically
  abort the simulation under such conditions rather than log it as
  an error (which might go undetected if the user is not using logging).
  The ``NS_ABORT_MSG_IF/UNLESS(cond,msg)`` macros and variants, as well
  as the lower-level ``NS_FATAL_ERROR(msg)`` macro, can be used to terminate
  the simulation with an error message.

* Use ``NS_LOG_WARN`` for unusual conditions that are not considered
  invalid.  An example might be that some resource has been exhausted
  (e.g., the DHCP server has run out of addresses to allocate).
  Please give some hints as to the nature of the problem and how
  it might be corrected.

* Use ``NS_LOG_INFO`` for events that cause a state change in the model.
  Avoid using it for logging periodic events that are not causing a
  state change (e.g., a Wi-Fi beacon is sent, but all nodes are already
  associated to the access point).  Try to be efficient in using it;
  for instance, sending a message is usually an important state change event,
  but try to capture this event with one single log message at the ``INFO``
  level rather than multiple.  If multiple log messages are desired to
  fully capture the event and all of its consequences,  use ``DEBUG`` level
  for the additional messages.  The intent of this log level is to allow a
  user to examine the normal operation of a model without becoming overwhelmed
  by the output.

* ``NS_LOG_LOGIC`` is used to trace important logic branches or decision points
  within a function, without dumping all details of the variable states,
  called function return values, individual iterations, etc.  It may be useful
  to think of it as a less granular level of function logging than ``DEBUG,``
  and may not be used by all models (some authors may choose to only use
  ``DEBUG`` level for full logging).

* ``NS_LOG_DEBUG`` is usually used for full voluminous debugging, and contains
  much more information than ``NS_LOG_INFO``, such as the detailed execution
  logic of functions and the values that variables take within those functions.

* Test that your logging changes do not break the code.
  Run some example programs with all log components turned on (e.g.
  ``NS_LOG="***"``).

* Use a unary operator (preferred) or an explicit cast for any variable of type uint8_t or int8_t,
  e.g., ``NS_LOG_DEBUG("Variable i is " << +i);``.
  e.g., ``NS_LOG_DEBUG("Variable i is " << static_cast<int>(i));`` or
  Without the cast, the integer is interpreted as a char, and the result
  will be most likely not in line with the expectations.
  This is a well documented C++ 'feature'.

Controlling timestamp precision
*******************************

Timestamps are printed out in units of seconds.  When used with the default
|ns3| time resolution of nanoseconds, the default timestamp precision is 9
digits, with fixed format, to allow for 9 digits to be consistently printed
to the right of the decimal point.  Example:

::

  +0.000123456s RandomVariableStream:SetAntithetic(0x805040, 0)

When the |ns3| simulation uses higher time resolution such as picoseconds
or femtoseconds, the precision is expanded accordingly; e.g. for picosecond:

::

  +0.000123456789s RandomVariableStream:SetAntithetic(0x805040, 0)

When the |ns3| simulation uses a time resolution lower than microseconds,
the default C++ precision is used.

An example program at ``src/core/examples/sample-log-time-format.cc``
demonstrates how to change the timestamp formatting.

The maximum useful precision is 20 decimal digits, since Time is signed 64
bits.


Asserts
*******

The |ns3| assert facility can be used to validate that invariant conditions
are met during execution. If the condition is not met an error message is given
and the program stops, printing the location of the failed assert.

The assert implementation is enabled in ``debug`` and ``default``
builds, but disabled in all other build profiles to improve execution speed.

How to add asserts to your code
===============================

There is only one macro one should use::

  NS_ASSERT_MSG(condition, message);

The ``condition`` should be the invariant you want to test, as a
boolean expression.  The ``message`` should explain what the
condition means and/or the possible source of the error.

There is a variant available without a message, ``NS_ASSERT(condition)``,
but we recommend using the message variant in ns-3 library code,
as a well-crafted message can help users figure out how to fix the underlying
issue with their script.

In either case if the condition evaluates to ``false`` the assert will print
an error message to ``std::cerr`` containing the following
information:

* Error message: "NS_ASSERT failed, "
* The ``condition`` expression: "cond="``condition``"
* The ``message``: "msg="``message``"
* The simulation time and node, as would be printed by logging.
  These are printed independent of the flags or prefix set on any
  logging component.
* The file and line containing the assert: "file=``file``, line=``line``

Here is an example which doesn't assert:

.. sourcecode:: bash

   $ ./ns3 run assert-example
   [0/2] Re-checking globbed directories...
   ninja: no work to do.
   NS_ASSERT_MSG example
     if an argument is given this example will assert.

and here is an example which does:

.. sourcecode:: bash

   $ ./ns3 run assert-example -- foo
   [0/2] Re-checking globbed directories...
   ninja: no work to do.
   NS_ASSERT_MSG example
     if an argument is given this example will assert.
   NS_ASSERT failed, cond="argc == 1", msg="An argument was given, so we assert", file=/Users/barnes26/Code/netsim/ns3/repos/ns-3-dev/src/core/examples/assert-example.cc, line=44
   NS_FATAL, terminating
   terminate called without an active exception
   Command 'build/debug/src/core/examples/ns3-dev-assert-example-debug foo' died with <Signals.SIGABRT: 6>.

You can try the example program `assert-example.cc` in `src/core/example`
with or without arguments to see the action of ``NS_ASSERT_MSG``.
