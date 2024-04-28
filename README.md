# Funtion parameter nodes spawner plugin for the Unreal Engine editor

A tiny plugin, that I made to automate the creation of function parameters nodes, while scripting in blueprints.
The main motivation for it, was to save me time, while editing blueprint functions.

It is really tedious to create function parameter/input nodes inside of a blueprint graph.
You basically need to look for them in the context menu among hundreds of other entries there.
Typing the name of the parameter helps of course, however you need to remember how to spell it.
In any case, it takes way more time to make a getter of a function parameter, than of a property or a local variable, because you can just Drag&Drop them into the graph from some the side panels in the Editor.

What this plugin does is to merely create a getter node for each of the inputs of the function, with a single mouse click.
More precisely, the plugin attaches to the Editor and starts listening for user input.
A specific sequence key strokes activates it and the next left mouse click somewhere in the graph automatically creates the nodes.
The default activation sequence is just the key 'A' on the keyboard.
So, if you tap 'A' and then left-click inside a funciton graph, the nodes should appear under your pointer.
The activation sequence can be configured by simply adding the following lines to the _DefaultEditor.ini_ file:

```
[/Script/FunctionParamNodesSpawner.EdFunctionParamNodesSpawner]
ActivationSequence="arg"
```

The code above will set the activation sequence to 'arg'. So all of the 'A', 'R' and 'G' keys need to be clicked in a row, followed by a left-mouse click, in order to generate the input nodes.

Installing the plugin is as simple as cloning this repository into the _Plugins_ folder of you project. I developed it using UE 5.2, but it should be working with newer versions of Unreal, too.
