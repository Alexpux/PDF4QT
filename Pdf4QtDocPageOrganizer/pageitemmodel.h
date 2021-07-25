//    Copyright (C) 2021 Jakub Melka
//
//    This file is part of Pdf4Qt.
//
//    Pdf4Qt is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    with the written consent of the copyright owner, any later version.
//
//    Pdf4Qt is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with Pdf4Qt.  If not, see <https://www.gnu.org/licenses/>.

#ifndef PDFDOCPAGEORGANIZER_PAGEITEMMODEL_H
#define PDFDOCPAGEORGANIZER_PAGEITEMMODEL_H

#include "pdfdocument.h"
#include "pdfutils.h"
#include "pdfdocumentmanipulator.h"

#include <QImage>
#include <QAbstractItemModel>

namespace pdfdocpage
{

enum PageType
{
    PT_DocumentPage,
    PT_Image,
    PT_Empty
};

struct PageGroupItem
{
    QString groupName;
    QString pagesCaption;
    QStringList tags;

    struct GroupItem
    {
        auto operator<=>(const GroupItem&) const = default;

        int documentIndex = -1;
        pdf::PDFInteger pageIndex = -1;
        pdf::PDFInteger imageIndex = -1;
        QSizeF rotatedPageDimensionsMM; ///< Rotated page dimensions, but without additional rotation
        pdf::PageRotation pageAdditionalRotation = pdf::PageRotation::None; ///< Additional rotation applied to the page
        PageType pageType = PT_DocumentPage;
    };

    std::vector<GroupItem> groups;

    auto operator<=>(const PageGroupItem&) const = default;

    bool isGrouped() const { return groups.size() > 1; }

    std::set<int> getDocumentIndices() const;

    void rotateLeft();
    void rotateRight();
};

struct DocumentItem
{
    QString fileName;
    pdf::PDFDocument document;
};

struct ImageItem
{
    QImage image;
    QByteArray imageData;
};

class PageItemModel : public QAbstractItemModel
{
    Q_OBJECT

private:
    using BaseClass = QAbstractItemModel;

public:
    explicit PageItemModel(QObject* parent);

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    virtual QModelIndex index(int row, int column, const QModelIndex& parent) const override;
    virtual QModelIndex parent(const QModelIndex& index) const override;
    virtual int rowCount(const QModelIndex& parent) const override;
    virtual int columnCount(const QModelIndex& parent) const override;
    virtual QVariant data(const QModelIndex& index, int role) const override;
    virtual QStringList mimeTypes() const override;
    virtual QMimeData* mimeData(const QModelIndexList& indexes) const override;
    virtual bool canDropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) const override;
    virtual bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) override;
    virtual Qt::DropActions supportedDropActions() const override;
    virtual Qt::DropActions supportedDragActions() const override;
    virtual Qt::ItemFlags flags(const QModelIndex& index) const override;

    enum class AssembleMode
    {
        Unite,
        Separate,
        SeparateGrouped
    };

    std::vector<std::vector<pdf::PDFDocumentManipulator::AssembledPage>> getAssembledPages(AssembleMode mode) const;

    /// Clear all data and undo/redo
    void clear();

    /// Adds document to the model, inserts one single page group containing
    /// the whole document. Returns index of a newly added document. If document
    /// cannot be added (for example, it already exists), -1 is returned.
    /// \param fileName File name
    /// \param document Document
    /// \returns Identifier of the document (internal index)
    int addDocument(QString fileName, pdf::PDFDocument document);

    /// Adds image to the model, inserts one single page containing
    /// the image. Returns index of a newly added image. If image
    /// cannot be read from the file, -1 is returned.
    /// \param fileName Image file
    /// \param index Index, where image is inserted
    /// \returns Identifier of the image (internal index)
    int insertImage(QString fileName, const QModelIndex& index);

    /// Returns item at a given index. If item doesn't exist,
    /// then nullptr is returned.
    /// \param index Index
    const PageGroupItem* getItem(const QModelIndex& index) const;

    /// Returns item at a given index. If item doesn't exist,
    /// then nullptr is returned.
    /// \param index Index
    PageGroupItem* getItem(const QModelIndex& index);

    ///  Returns true, if grouped item exists in the indices
    bool isGrouped(const QModelIndexList& indices) const;

    /// Returns true, if trash bin is empty
    bool isTrashBinEmpty() const { return m_trashBin.empty(); }

    QItemSelection getSelectionEven() const;
    QItemSelection getSelectionOdd() const;
    QItemSelection getSelectionPortrait() const;
    QItemSelection getSelectionLandscape() const;

    void group(const QModelIndexList& list);
    void ungroup(const QModelIndexList& list);

    QModelIndexList restoreRemovedItems();
    QModelIndexList cloneSelection(const QModelIndexList& list);
    void removeSelection(const QModelIndexList& list);
    void insertEmptyPage(const QModelIndexList& list);

    void rotateLeft(const QModelIndexList& list);
    void rotateRight(const QModelIndexList& list);

    static QString getMimeDataType() { return QLatin1String("application/pagemodel.PDF4QtDocPageOrganizer"); }

    const std::map<int, DocumentItem>& getDocuments() const { return m_documents; }
    const std::map<int, ImageItem>& getImages() const { return m_images; }

private:
    void createDocumentGroup(int index);
    QString getGroupNameFromDocument(int index) const;
    void updateItemCaptionAndTags(PageGroupItem& item) const;
    void insertEmptyPage(const QModelIndex& index);

    QItemSelection getSelectionImpl(std::function<bool(const PageGroupItem::GroupItem&)> filter) const;

    std::vector<PageGroupItem> m_pageGroupItems;
    std::map<int, DocumentItem> m_documents;
    std::map<int, ImageItem> m_images;
    std::vector<PageGroupItem> m_trashBin;
};

}   // namespace pdfdocpage

#endif // PDFDOCPAGEORGANIZER_PAGEITEMMODEL_H
