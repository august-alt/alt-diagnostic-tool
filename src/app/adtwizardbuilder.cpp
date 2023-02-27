/***********************************************************************************************************************
**
** Copyright (C) 2023 BaseALT Ltd. <org@basealt.ru>
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**
***********************************************************************************************************************/

#include "adtwizardbuilder.h"
#include "../core/adtjsonloader.h"

#include <QMessageBox>

ADTWizardBuilder &ADTWizardBuilder::withFile(QString &filename)
{
    m_dataFile = filename;
    return *this;
}

ADTWizardBuilder &ADTWizardBuilder::withChecksSection(QString &checksSectionName)
{
    m_checksSection = checksSectionName;
    return *this;
}

ADTWizardBuilder &ADTWizardBuilder::withResolversSections(QString &resolversSectionName)
{
    m_resolversSection = resolversSectionName;
    return *this;
}

ADTWizardBuilder &ADTWizardBuilder::withService(QString &serviceName)
{
    m_serviceName = serviceName;
    return *this;
}

ADTWizardBuilder &ADTWizardBuilder::withPath(QString &path)
{
    m_path = path;
    return *this;
}

ADTWizardBuilder &ADTWizardBuilder::withInterface(QString &interfaceName)
{
    m_interfaceName = interfaceName;
    return *this;
}

std::unique_ptr<ADTWizard> ADTWizardBuilder::build()
{
    QJsonDocument checks    = ADTJsonLoader::loadDocument(m_dataFile, m_checksSection);
    QJsonDocument resolvers = ADTJsonLoader::loadDocument(m_dataFile, m_resolversSection);

    if (checks.isEmpty())
    {
        QMessageBox checksMsgBox;
        checksMsgBox.setText(
            QObject::tr("Checks file is missing or corrupted. Cannot continue working!"));
        checksMsgBox.setIcon(QMessageBox::Critical);
        checksMsgBox.exec();
        return nullptr;
    }

    if (resolvers.isEmpty())
    {
        QMessageBox resolversMsgBox;
        resolversMsgBox.setText(QObject::tr(
            "The data file does not contain data about the resolvers, only checks are possible."));
        resolversMsgBox.setIcon(QMessageBox::Warning);
        resolversMsgBox.exec();
    }

    return std::make_unique<ADTWizard>(checks, resolvers, m_serviceName, m_path, m_interfaceName);
}