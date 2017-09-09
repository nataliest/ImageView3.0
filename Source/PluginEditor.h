/*
 ==============================================================================
 
 This file was auto-generated!
 
 It contains the basic framework code for a JUCE plugin editor.
 
 ==============================================================================
 */

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
 */
class DragAndDrop;
class ImageViewAudioProcessorEditor  : public AudioProcessorEditor,
public FileBrowserListener, public DragAndDropContainer, public DragAndDropTarget,
public FileDragAndDropTarget, private Button::Listener
{
public:
    ImageViewAudioProcessorEditor (ImageViewAudioProcessor&);
    ~ImageViewAudioProcessorEditor();
    //friend class DragAndDrop;
    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void selectionChanged() override;
    
    void browserRootChanged (const File& f) override;
    
    //==============================================================================
    // These methods implement the FileDragAndDropTarget interface, and allow our component
    // to accept drag-and-drop of files..
    
    bool isInterestedInFileDrag (const StringArray& /*files*/) override
    {
        // normally you'd check these files to see if they're something that you're
        // interested in before returning true, but for the demo, we'll say yes to anything..
        return true;
    }
    
    void fileDragEnter (const StringArray& /*files*/, int /*x*/, int /*y*/) override
    {
        smthIsBeingDraggedOver = true;
        repaint();
    }
    
    void fileDragMove (const StringArray& /*files*/, int /*x*/, int /*y*/) override
    {
    }
    
    void fileDragExit (const StringArray& /*files*/) override
    {
        smthIsBeingDraggedOver = false;
        repaint();
    }
    
    // this one is called in the plug-in
    void filesDropped (const StringArray& files, int /*x*/, int /*y*/) override ;
    void itemDropped (const SourceDetails& dragSourceDetails) override
    {
        
    }
    
    bool isInterestedInDragSource (const SourceDetails& /*dragSourceDetails*/) override
    {
        // normally you'd check the sourceDescription value to see if it's the
        // sort of object that you're interested in before returning true, but for
        // the demo, we'll say yes to anything..
        return true;
    }
    
    void itemDragEnter (const SourceDetails& /*dragSourceDetails*/) override
    {
        smthIsBeingDraggedOver = true;
        repaint();
    }
    
    void itemDragMove (const SourceDetails& /*dragSourceDetails*/) override
    {
    }
    
    void itemDragExit (const SourceDetails& /*dragSourceDetails*/) override
    {
        smthIsBeingDraggedOver = false;
        repaint();
    }
    
    void buttonClicked (Button* b) override;
    DragAndDrop* dragDropComponent;
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    //ImageViewAudioProcessor& processor;
    
    TextButton* selectButton;
    StretchableLayoutResizerBar resizerTop;
    ImageComponent imagePreview;
    StretchableLayoutResizerBar resizerBottom;
    TextEditor textbox;
    
    String pathOrFile;
    bool smthIsBeingDraggedOver;
    
    WildcardFileFilter* fileFilter;
    TimeSliceThread fileDirectoryThread;
    DirectoryContentsList dirContentsList;
    StretchableLayoutManager layout;
    FileTreeComponent fileTree;
    
    void fileClicked (const File&, const MouseEvent&) override {}
    void fileDoubleClicked (const File&) override {}
    
    
    ImageViewAudioProcessor& getProcessor() const
    {
        return static_cast<ImageViewAudioProcessor&> (processor);
    }
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ImageViewAudioProcessorEditor)
};

class DragAndDrop :
public Component,
public DragAndDropTarget,
public FileDragAndDropTarget,
public TextDragAndDropTarget
{
public:
    
    
    DragAndDrop(const String& mypath): message("Click \"Select Directory\" button or drag and drop folder or files to copy!\nCurrent Directory: " + String(mypath)), path(mypath) {
        
    }
    
    void paint (Graphics& g) override
    {
        g.fillAll (Colours::black);
        
//        // draw a red line around the comp if the user's currently dragging something over it..
//        if (somethingIsBeingDraggedOver)
//        {
//            g.setColour (Colours::darkgrey);
//            g.drawRect (getLocalBounds(), 0.5);
//            //g.drawFittedText ("", getLocalBounds().reduced (10, 0), Justification::centred, 4);
//        }
        
        g.setColour (Colours::white);
        g.setFont(Font("Verdana", 14.0f, Font::plain));
        g.drawFittedText (message, getLocalBounds().reduced (10, 0), Justification::centred, 4);
        
    }
    
    //==============================================================================
    // These methods implement the DragAndDropTarget interface, and allow our component
    // to accept drag-and-drop of objects from other Juce components..
    
    bool isInterestedInDragSource (const SourceDetails& /*dragSourceDetails*/) override
    {
        // normally you'd check the sourceDescription value to see if it's the
        // sort of object that you're interested in before returning true, but for
        // the demo, we'll say yes to anything..
        return true;
    }
    
    void itemDragEnter (const SourceDetails& /*dragSourceDetails*/) override
    {
        somethingIsBeingDraggedOver = true;
        repaint();
    }
    
    void itemDragMove (const SourceDetails& /*dragSourceDetails*/) override
    {
    }
    
    void itemDragExit (const SourceDetails& /*dragSourceDetails*/) override
    {
        somethingIsBeingDraggedOver = false;
        repaint();
    }
    
    void itemDropped (const SourceDetails& dragSourceDetails) override
    {
        
        message = "Click \"Select directory\" button or drag and drop folder or files to copy!\nCurrent Directory: " + dragSourceDetails.description.toString();
        path = dragSourceDetails.description.toString();
        somethingIsBeingDraggedOver = false;
        repaint();
    }
    
    //==============================================================================
    // These methods implement the FileDragAndDropTarget interface, and allow our component
    // to accept drag-and-drop of files..
    
    bool isInterestedInFileDrag (const StringArray& /*files*/) override
    {
        // normally you'd check these files to see if they're something that you're
        // interested in before returning true, but for the demo, we'll say yes to anything..
        return true;
    }
    
    void fileDragEnter (const StringArray& /*files*/, int /*x*/, int /*y*/) override
    {
        somethingIsBeingDraggedOver = true;
        repaint();
    }
    
    void fileDragMove (const StringArray& /*files*/, int /*x*/, int /*y*/) override
    {
    }
    
    void fileDragExit (const StringArray& /*files*/) override
    {
        somethingIsBeingDraggedOver = false;
        repaint();
    }
    
    // this one is called in the plug-in
    void filesDropped (const StringArray& files, int /*x*/, int /*y*/) override
    {
        File file;
        
        if (files.size() == 1) {
            file = files[0];
            
            if (file.isDirectory()) { // update path only if 1 directory is dropped
                path = files.joinIntoString("\n");
            } else {
                copyFilesToCurrentDirectory(files);
            }
            
        } else if (files.size() > 0) {
            copyFilesToCurrentDirectory(files);
        }
        
        message = "Click \"Select directory\" button or drag and drop folder or files to copy!\nCurrent Directory: " + path;
        
        somethingIsBeingDraggedOver = false;
        repaint();
    }
    
    //==============================================================================
    // These methods implement the TextDragAndDropTarget interface, and allow our component
    // to accept drag-and-drop of text..
    
    bool isInterestedInTextDrag (const String& /*text*/) override
    {
        return true;
    }
    
    void textDragEnter (const String& /*text*/, int /*x*/, int /*y*/) override
    {
        somethingIsBeingDraggedOver = true;
        repaint();
    }
    
    void textDragMove (const String& /*text*/, int /*x*/, int /*y*/) override
    {
    }
    
    void textDragExit (const String& /*text*/) override
    {
        somethingIsBeingDraggedOver = false;
        repaint();
    }
    
    void textDropped (const String& text, int /*x*/, int /*y*/) override
    {
        message = "Text dropped:\n" + text;
        
        somethingIsBeingDraggedOver = false;
        repaint();
    }
    
    String getMess() const {
        return message;
    }
    
    String getPath() const {
        return path;
    }
    
    void setPath(const String& newPath) {
        path = newPath;
    }
    
    void setMess(const String& newMess) {
        message = newMess;
    }
    
    void copyFilesToCurrentDirectory(const StringArray& files) {
        File file;
        String destinationPath;
        String ext;
        String pathToCopy = path;
        File pathCopy = path;
        File checkFile;
        
        for (int i = 0; i < files.size(); ++i) {
            file = files[i];
            checkFile = pathToCopy + "/" + file.getFileName();
            if (!file.isDirectory()) {
                ext = file.getFileExtension();
                if (pathCopy.hasWriteAccess() && (ext == ".jpeg" || ext == ".jpg" || ext == ".png" || ext == ".gif" || ext == "*")) {
                    if (checkFile.existsAsFile()) {
                        destinationPath = pathToCopy + "/" + file.getFileNameWithoutExtension() + "_copy" + ext;
                    } else {
                        destinationPath = pathToCopy + "/" + file.getFileName();
                    }
                    
                    filetreeChanged = file.copyFileTo(destinationPath);
                }
            }
        }
        
        //        for (int i = 0; i < files.size(); ++i) {
        //            file = files[i];
        //
        //            ext = file.getFileExtension();
        //            if (pathCopy.hasWriteAccess()) {
        //                destinationPath = pathToCopy + "/" + file.getFileNameWithoutExtension() + "_copy" + ext;
        //                filetreeChanged = file.copyFileTo(destinationPath);
        //
        //            }
        //        }
    }
    
    bool getFiletreeChanged() const {
        return filetreeChanged;
    }
    
    void setFiletreeChanged(bool newBool) {
        filetreeChanged = newBool;
    }
    
private:

    bool filetreeChanged;
    String message;
    String path;
    StretchableLayoutManager layout;
    bool somethingIsBeingDraggedOver;
};

#endif  // PLUGINEDITOR_H_INCLUDED


