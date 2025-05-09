/*
 * Copyright (c) 2009 INRIA
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Author: Mathieu Lacage <mathieu.lacage@cutebugs.net>
 */

#include "config-store.h"

#include "raw-text-config.h"

#include "ns3/abort.h"
#include "ns3/attribute-construction-list.h"
#include "ns3/boolean.h"
#include "ns3/enum.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/string.h"
#ifdef HAVE_LIBXML2
#include "xml-config.h"
#endif

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("ConfigStore");

NS_OBJECT_ENSURE_REGISTERED(ConfigStore);

TypeId
ConfigStore::GetTypeId()
{
    static TypeId tid =
        TypeId("ns3::ConfigStore")
            .SetParent<ObjectBase>()
            .SetGroupName("ConfigStore")
            .AddAttribute("Mode",
                          "Configuration mode",
                          EnumValue(ConfigStore::NONE),
                          MakeEnumAccessor<Mode>(&ConfigStore::SetMode),
                          MakeEnumChecker(ConfigStore::NONE,
                                          "None",
                                          ConfigStore::LOAD,
                                          "Load",
                                          ConfigStore::SAVE,
                                          "Save"))
            .AddAttribute("Filename",
                          "The file where the configuration should be saved to or loaded from.",
                          StringValue(""),
                          MakeStringAccessor(&ConfigStore::SetFilename),
                          MakeStringChecker())
            .AddAttribute(
                "FileFormat",
                "Type of file format",
                EnumValue(ConfigStore::RAW_TEXT),
                MakeEnumAccessor<FileFormat>(&ConfigStore::SetFileFormat),
                MakeEnumChecker(ConfigStore::RAW_TEXT, "RawText", ConfigStore::XML, "Xml"))
            // NS_DEPRECATED_3_43
            .AddAttribute(
                "SaveDeprecated",
                "Save DEPRECATED attributes",
                BooleanValue(true),
                MakeBooleanAccessor(&ConfigStore::SetSaveDeprecated),
                MakeBooleanChecker(),
                TypeId::SupportLevel::OBSOLETE,
                "OBSOLETE since ns-3.43 as it is no longer needed; deprecated attributes are saved "
                "only if their value differs from their respective original initial value");
    return tid;
}

TypeId
ConfigStore::GetInstanceTypeId() const
{
    return GetTypeId();
}

ConfigStore::ConfigStore()
{
    NS_LOG_FUNCTION(this);
    ObjectBase::ConstructSelf(AttributeConstructionList());

#ifdef HAVE_LIBXML2
    if (m_fileFormat == ConfigStore::XML)
    {
        if (m_mode == ConfigStore::SAVE)
        {
            m_file = new XmlConfigSave();
        }
        else if (m_mode == ConfigStore::LOAD)
        {
            m_file = new XmlConfigLoad();
        }
        else
        {
            m_file = new NoneFileConfig();
        }
    }
#else
    if (m_fileFormat == ConfigStore::XML)
    {
        if (m_mode == ConfigStore::SAVE || m_mode == ConfigStore::LOAD)
        {
            NS_ABORT_MSG(
                "ConfigStore tried to read or write an XML file but XML is not supported.");
        }
        else
        {
            m_file = new NoneFileConfig();
        }
    }
#endif /* HAVE_LIBXML2 */

    if (m_fileFormat == ConfigStore::RAW_TEXT)
    {
        if (m_mode == ConfigStore::SAVE)
        {
            m_file = new RawTextConfigSave();
        }
        else if (m_mode == ConfigStore::LOAD)
        {
            m_file = new RawTextConfigLoad();
        }
        else
        {
            m_file = new NoneFileConfig();
        }
    }
    m_file->SetFilename(m_filename);
    NS_LOG_FUNCTION(this << ": format: " << m_fileFormat << ", mode: " << m_mode
                         << ", file name: " << m_filename);
}

ConfigStore::~ConfigStore()
{
    NS_LOG_FUNCTION(this);
    delete m_file;
    m_file = nullptr;
}

void
ConfigStore::SetMode(Mode mode)
{
    NS_LOG_FUNCTION(this << mode);
    m_mode = mode;
}

void
ConfigStore::SetFileFormat(FileFormat format)
{
    NS_LOG_FUNCTION(this << format);
    m_fileFormat = format;
}

void
ConfigStore::SetFilename(std::string filename)
{
    NS_LOG_FUNCTION(this << filename);
    m_filename = filename;
}

void
ConfigStore::SetSaveDeprecated(bool saveDeprecated)
{
    NS_LOG_FUNCTION(this << saveDeprecated);
    m_saveDeprecated = saveDeprecated;
}

void
ConfigStore::ConfigureAttributes()
{
    NS_LOG_FUNCTION(this);
    m_file->Attributes();
}

void
ConfigStore::ConfigureDefaults()
{
    NS_LOG_FUNCTION(this);
    m_file->Default();
    m_file->Global();
}

std::ostream&
operator<<(std::ostream& os, ConfigStore::Mode& mode)
{
    switch (mode)
    {
    case ConfigStore::LOAD:
        os << "LOAD";
        break;
    case ConfigStore::SAVE:
        os << "SAVE";
        break;
    case ConfigStore::NONE:
        os << "NONE";
        break;
    default:
        os << "UNKNOWN";
    }
    return os;
}

std::ostream&
operator<<(std::ostream& os, ConfigStore::FileFormat& format)
{
    switch (format)
    {
    case ConfigStore::XML:
        os << "XML";
        break;
    case ConfigStore::RAW_TEXT:
        os << "RAW_TEXT";
        break;
    }
    return os;
}

} // namespace ns3
