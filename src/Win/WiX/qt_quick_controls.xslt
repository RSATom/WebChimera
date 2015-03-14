<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="2.0"
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:wix="http://schemas.microsoft.com/wix/2006/wi"
    xmlns="http://schemas.microsoft.com/wix/2006/wi"
    exclude-result-prefixes="wix"
>
    <xsl:output method="xml" indent="yes"/>

    <xsl:template match="@* | node()">
        <xsl:copy>
            <xsl:apply-templates select="@* | node()"/>
        </xsl:copy>
    </xsl:template>

    <xsl:template match="wix:Wix">
        <xsl:copy>
            <xsl:apply-templates/>
        </xsl:copy>
    </xsl:template>

    <xsl:template match="wix:DirectoryRef">
        <xsl:apply-templates />
    </xsl:template>

    <xsl:template match="wix:Directory">
        <xsl:param name="relative_parent_path"/>
        <xsl:variable name="relative_path">
            <xsl:choose>
                <xsl:when test="$relative_parent_path">
                    <xsl:value-of select="concat( $relative_parent_path, '\', @Name )"/>
                </xsl:when>
                <xsl:otherwise>
                    <xsl:value-of select="concat( 'qml\QtQuick', '\', @Name )"/>
                </xsl:otherwise>
            </xsl:choose>
        </xsl:variable>
        <xsl:variable name="full_path">
            <xsl:value-of select="concat( 'SOFTWARE\${PLUGIN_NAME}\${FBSTRING_PLUGIN_VERSION}', '\', $relative_path )"/>
        </xsl:variable>
        <xsl:variable name="relative_id">
            <xsl:value-of select="translate( $relative_path, '\', '_' )"/>
        </xsl:variable>
        <xsl:variable name="guid_offset">
            <xsl:number level="any" count="wix:Directory | wix:DirectoryRef"/>
        </xsl:variable>
        <xsl:variable name="guid">
            <xsl:value-of select="//wix:Component[@Guid][position()=$guid_offset]/@Guid"/>
        </xsl:variable>

        <xsl:copy>
            <xsl:attribute name="Id">
                <xsl:value-of select="$relative_id" />
            </xsl:attribute>
            <xsl:copy-of select="@Name" />
            <Component>
                <xsl:attribute name="Guid">
                    <xsl:value-of select="$guid" />
                </xsl:attribute>
                <xsl:attribute name="Id">
                    <xsl:value-of select="concat('c_QT_', $relative_id)" />
                </xsl:attribute>
                <RemoveFolder On="uninstall">
                    <xsl:attribute name="Id">
                        <xsl:value-of select="concat('u_', $relative_id)" />
                    </xsl:attribute>
                </RemoveFolder>
                <RegistryValue>
                    <xsl:attribute name="Root">HKCU</xsl:attribute>
                    <xsl:attribute name="Key">
                        <xsl:value-of select="$full_path" />
                    </xsl:attribute>
                    <xsl:attribute name="Name">QtUninstall</xsl:attribute>
                    <xsl:attribute name="Type">string</xsl:attribute>
                    <xsl:attribute name="Value">${Qt5Core_VERSION_STRING}</xsl:attribute>
                    <xsl:attribute name="KeyPath">yes</xsl:attribute>
                </RegistryValue>
                <xsl:apply-templates select="wix:Component/wix:File"/>
            </Component>
            <xsl:apply-templates select="wix:Directory">
                <xsl:with-param name="relative_parent_path" select="$relative_path"/>
            </xsl:apply-templates>
        </xsl:copy>
    </xsl:template>

    <xsl:template match="wix:Component">
        <xsl:apply-templates />
    </xsl:template>

    <xsl:template match="wix:File">
        <xsl:copy>
            <xsl:attribute name="Id">
                <xsl:value-of select="translate( concat( 'Controls', substring-after( @Source, 'SourceDir' ) ), '\@-', '___')" />
            </xsl:attribute>
            <xsl:attribute name="Source">
                <xsl:value-of select="concat( '$(var.QTDIR)\qml\QtQuick\Controls', substring-after( @Source, 'SourceDir' ) )" />
            </xsl:attribute>
        </xsl:copy>
    </xsl:template>

    <xsl:template match="wix:Wix/wix:Fragment[1]">
        <xsl:apply-templates />
    </xsl:template>

    <xsl:template match="wix:Wix/wix:Fragment[2]">
    </xsl:template>

</xsl:stylesheet>
