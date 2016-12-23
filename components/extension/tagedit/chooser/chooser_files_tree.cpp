 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include "chooser_files_tree.h"

Signal1<Void, const Directory &>	 BoCA::ChooserFilesTree::onSelectDirectory;

BoCA::ChooserFilesTree::ChooserFilesTree(const Directory &iDirectory) : Tree(iDirectory.GetDirectoryName())
{
	directory = iDirectory;

	if (directory.GetDirectoryName() == NIL) SetText(directory);

#ifndef __WIN32__
	if (GetText() == NIL) SetText("/");
#endif

	onOpen.Connect(&ChooserFilesTree::OnOpen, this);
	onClose.Connect(&ChooserFilesTree::OnClose, this);

	onSelect.Connect(&ChooserFilesTree::OnSelect, this);
}

BoCA::ChooserFilesTree::~ChooserFilesTree()
{
	foreach (Tree *tree, trees) DeleteObject(tree);
}

/* Called when the tree is opened.
 * ----
 * Fills the tree with subdirectory trees.
 */
Void BoCA::ChooserFilesTree::OnOpen()
{
	const Array<Directory>	&subDirectories = directory.GetDirectories();

	foreach (const Directory &currentDirectory, subDirectories)
	{
		Tree	*newTree = new ChooserFilesTree(currentDirectory);

		trees.Add(newTree);

		Add(newTree);
	}
}

/* Called when the tree is closed.
 * ----
 * Cleans up subdirectory trees.
 */
Void BoCA::ChooserFilesTree::OnClose()
{
	foreach (Tree *tree, trees) DeleteObject(tree);

	trees.RemoveAll();
}

/* Called when the entry is selected.
 * ----
 * Fires the onSelectDirectory signal.
 */
Void BoCA::ChooserFilesTree::OnSelect()
{
	onSelectDirectory.Emit(directory);;
}
