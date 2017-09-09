/*
 ==============================================================================
 
 This file was auto-generated!
 
 It contains the basic framework code for a JUCE plugin editor.
 
 ==============================================================================
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
ImageViewAudioProcessorEditor::ImageViewAudioProcessorEditor (ImageViewAudioProcessor& p)
: AudioProcessorEditor (p),
//processor (p), // not required if we have the getProcesson() methon implemented
//d(new DragAndDrop(*(p.filepath))), // could be initialized in the initialization list

fileFilter (new WildcardFileFilter("*.jpeg;*.jpg;*.png;*.gif", "*", "Image Filter")),
fileDirectoryThread ("Image File Scanner"),
dirContentsList (fileFilter, fileDirectoryThread),
fileTree (dirContentsList),
resizerTop(&layout, 3, false),
resizerBottom(&layout, 5, false)

{
    pathOrFile = *(p.filepath);
    File path = *(p.filepath);
    if (!path.isDirectory()){
        path.createDirectory();
    }

    dragDropComponent = new DragAndDrop(*(p.filepath));
    addAndMakeVisible(dragDropComponent);
    
    setOpaque (true);
    dirContentsList.setDirectory (pathOrFile, true, true);
    fileDirectoryThread.startThread (1);
    
    fileTree.addListener (this);
    fileTree.setColour(DirectoryContentsDisplayComponent::highlightColourId, Colours::grey.withAlpha(0.9f));
    fileTree.setColour (TreeView::backgroundColourId, Colours::whitesmoke.withAlpha (0.6f));

    
    selectButton = new TextButton("Select Directory");
    selectButton->addListener(this);
    selectButton->setColour(TextButton::buttonColourId, Colours::lightgrey);
    selectButton->setColour(TextButton::buttonOnColourId, Colours::black);
    selectButton->setColour(TextButton::textColourOnId, Colours::white);
    selectButton->setColour(TextButton::textColourOffId, Colours::black);
    addAndMakeVisible(selectButton);
    
    
    addAndMakeVisible (fileTree);
    
    addAndMakeVisible (resizerTop);
    
    addAndMakeVisible (imagePreview);
    
    addAndMakeVisible (resizerBottom);
    
    if (*(p.comments) != "") {
        textbox.setText(*(p.comments));
    }
    
    textbox.setColour(TextEditor::backgroundColourId, Colours::whitesmoke.withAlpha (0.6f));
    textbox.setColour(TextEditor::textColourId, Colours::black);
    textbox.setMultiLine(true);
    textbox.setReturnKeyStartsNewLine(true);
    textbox.setScrollbarsShown(true);
    textbox.setFont(Font("Verdana", 14.0f, Font::plain));
    textbox.setTextToShowWhenEmpty("Add your comments here...", Colours::darkgrey);
    addAndMakeVisible (textbox);
    
    // d&d
    layout.setItemLayout (0, -0.1, -0.1, -0.1);
    
    
    // button
    layout.setItemLayout (1, -0.05, -0.05, -0.05);
    
    // fileTree
    layout.setItemLayout (2, -0.1, -0.9, -0.182);
    
    // resizerTop
    layout.setItemLayout (3, 3, 3, 3);
    
    // imagePreview
    layout.setItemLayout (4, -0.1, -0.9, -0.618);
    
    // resizerBottom
    layout.setItemLayout (5, 3, 3, 3);
    
    // textbox
    layout.setItemLayout (6, -0.1, -0.5, -0.05);
    selectButton->changeWidthToFitText();
    setResizable(true, true);
    
    
    setSize (p.lastUIWidth, p.lastUIHeight);
    setResizeLimits (400, 400, 1200, 600);
}

ImageViewAudioProcessorEditor::~ImageViewAudioProcessorEditor()
{
    if (textbox.getText() != *(getProcessor().comments)) {
        *(getProcessor().comments) = textbox.getText();
    }
    
    
    delete selectButton;
    delete fileFilter;
    
    delete dragDropComponent;
}

//==============================================================================
void ImageViewAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::black);   // clear the background
    
    g.setColour (Colours::black);
    g.drawRect (getLocalBounds(), 0.5);
    g.setColour (Colours::white);
    
    g.setFont(Font("Verdana", 44.0f, Font::plain));
    g.drawFittedText ("ImageView", getLocalBounds().reduced (10, 0), Justification::centred, 4);
    browserRootChanged(dragDropComponent->getPath());
    *(getProcessor().filepath) = dragDropComponent->getPath();
    if (dragDropComponent->getFiletreeChanged()) {
        dirContentsList.refresh();
        
        dragDropComponent->setFiletreeChanged(false);
    }
}

void ImageViewAudioProcessorEditor::resized()
{
    // subcomponents in your editor..
    Rectangle<int> r (getLocalBounds());
    //
    // make a list of two of our child components that we want to reposition
    Component* comps[] = {  dragDropComponent,  selectButton, &fileTree, &resizerTop, &imagePreview, &resizerBottom, &textbox };
    
    // this will position the 3 components, one above the other, to fit
    // vertically into the rectangle provided.
    layout.layOutComponents (comps, 7, r.getX(), r.getY(), r.getWidth(), r.getHeight(), true, true);
    
    getProcessor().lastUIWidth = getWidth();
    getProcessor().lastUIHeight = getHeight();
    
}

void ImageViewAudioProcessorEditor::selectionChanged() {
    
    const File selectedFile (fileTree.getSelectedFile(0));
    
    if (selectedFile.existsAsFile())
        imagePreview.setImage (ImageCache::getFromFile (selectedFile));
}

void ImageViewAudioProcessorEditor::browserRootChanged (const File& f)  {
    dirContentsList.setDirectory (f, true, true);
    String newPath = f.getFullPathName();
}

void ImageViewAudioProcessorEditor::filesDropped (const StringArray& files, int /*x*/, int /*y*/)
{
    File file;
    if (files.size() == 1) {
        file = files[0];
        if (file.isDirectory()) { // update path only if 1 directory is dropped
            dragDropComponent->setPath(files.joinIntoString("\n"));
            Image newImage;
            imagePreview.setImage(newImage);
        } else {
            dragDropComponent->copyFilesToCurrentDirectory(files);
        }
    } else if (files.size() > 0) {
        dragDropComponent->copyFilesToCurrentDirectory(files);
    }
    dragDropComponent->setMess("Click \"Select Directory\" button or drag and drop folder or files to copy!\nCurrent Directory: " + dragDropComponent->getPath());
    
    smthIsBeingDraggedOver = false;
    repaint();
}

void ImageViewAudioProcessorEditor::buttonClicked (Button* b)
{
    if (b == selectButton)
    {
        WildcardFileFilter wildcardFilter ("*", String(), "Files");
        FileBrowserComponent browser (FileBrowserComponent::saveMode
                                      | FileBrowserComponent::canSelectDirectories
                                      | FileBrowserComponent::warnAboutOverwriting
                                      | FileBrowserComponent::canSelectFiles
                                      | FileBrowserComponent::filenameBoxIsReadOnly,
                                      File(dragDropComponent->getPath()),
                                      &wildcardFilter,
                                      nullptr);
        FileChooserDialogBox dialogBox ("Select Directory",
                                        "Please highlight and choose a folder you want to reference from the plug-in...",
                                        browser,
                                        false,
                                        browser.findColour (AlertWindow::backgroundColourId));
        dialogBox.setVisible(true);
        dialogBox.showAt(555, 164, 350, getProcessor().lastUIHeight);
        
        
        if (browser.getNumSelectedFiles() == 1 && browser.getSelectedFile(0).isDirectory()) {
            String newPathHighlighted = browser.getHighlightedFile().getFullPathName();
            if (newPathHighlighted != "" && browser.getHighlightedFile().getFileExtension() == "") {
                dragDropComponent->setPath(newPathHighlighted);
                Image newImage;
                imagePreview.setImage(newImage);
            }
            dragDropComponent->setMess("Click \"Select Directory\" button or drag and drop folder or files to copy!\nCurrent Directory: " + dragDropComponent->getPath());
        }
        else {
            dragDropComponent->setMess("Click \"Select Directory\" button or drag and drop folder or files to copy!\nCurrent Directory: " + dragDropComponent->getPath());
        }
        
    }
    
}
