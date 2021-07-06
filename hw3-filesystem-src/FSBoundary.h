#pragma once
#include "FSOperation.h"
#include <QStandardItemModel>
#include <stack>
#include <vector>
#include <sstream>
#include <QMessageBox>
using namespace std;

struct ui_tree_node {
	QString name;
	dd clusterId;
};

class FSBoundary{
public:
	static dd currentDirCluster;
	static fs_VirtualDisk* vdisk;
	static db* fsBuffer;
	static fat32_sFileDirEntry* fde;
	static fat32_sFileDirEntry* fde2;
	static fat32_sFileDirEntry* fdSelected;
	static FSOperation* fs;

	static vector<ui_tree_node>* path_route;
	static dd fbCheckFileNameValid(QString v);
	static string fbGetDevInfo();

	static void fbGoParent();
	static void fbGoto(ui_tree_node x);
	static QString fqConvert(db* p,dd digits=-1);
	static void fbGetDirList(QStandardItemModel** p, dd fileCluster);

	static void fbRouting(string path);
	static dd fbGetAvaSpace();

};

