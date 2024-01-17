# Problem

An app is given a config file, which contains records describing a category of item. Every category consists of two values: "Label" and "Value". "Label"s are not necessarily unique but their combination with the "Value" is uniqe, e.g. 

```
Label Value
A     1
A     2
``` 

is valid, whereas 

```
Label Value
A     1
A     1
``` 
is not a valid configuration. Along with "Label" and "Value" there are also other properties of the config, corresponding to the properties of the data records incoming to the app from the second source (more on that in a second). The value in such a column/property is either null or a "check" written in our company-internal language-like code. This check needs to pass so that an "Item" (the data record from the second source) can be labeled accordingly. So, for example


```
Label Value FileName
A     1     <some_regex>
A     2
``` 

If an app is given this config and the incoming Item would look like this:

```
Item:
   some_property: 5,
   FileName: "some string passing the regex"
```

it would be labeled as **A1** but if the FileName property were to fail on the FileName test, then it would be labeled as **A2**.

# Test Results

```
The configuration ( tests )
A 1: (test) 2: (test2)
B 1: (B)
L3 1: (test)
L4 1: (test)

The items, with label values
Item I1 filename, test
A:0     A:X     B:X     L3:0    L4:0
Item I2 filename, B
A:X     A:X     B:0     L3:X    L4:X
Item I3 filename, test2
A:X     A:1     B:X     L3:X    L4:X
Item I4 filename, X
A:X     A:X     B:X     L3:X    L4:X
```

# Performance
```
Timing for 49950 tests
raven::set::cRunWatch code timing profile
Calls           Mean (secs)     Total           Scope
       1        0.0916656       0.0916656       testLabels
```

