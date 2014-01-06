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
            <xsl:copy-of select="@*"/>
            <xsl:apply-templates/>
            <Fragment>
                <ComponentGroup Id="VLC">
                    <ComponentRef Id="c_VLC"/>
                        <xsl:for-each select="//wix:Directory">
                        <ComponentRef>
                            <xsl:attribute name="Id">
                                <xsl:value-of select="concat('c_',@Id)" />
                            </xsl:attribute>
                        </ComponentRef>
                    </xsl:for-each>
                </ComponentGroup>
            </Fragment>
        </xsl:copy>
    </xsl:template>

    <xsl:template match="/wix:Wix/wix:Fragment[1]/wix:DirectoryRef | wix:Directory">
        <xsl:param name="parent_path"/>
        <xsl:variable name="full_path">
            <xsl:choose>
                <xsl:when test="$parent_path">
                    <xsl:value-of select="concat($parent_path, '\', @Name)"/>
                </xsl:when>
                <xsl:otherwise>
                    <xsl:value-of select="'SOFTWARE\${COMPANY_NAME}\${PLUGIN_NAME}\${FBSTRING_PLUGIN_VERSION}'"/>
                </xsl:otherwise>
            </xsl:choose>
        </xsl:variable>
        <xsl:variable name="guid_offset">
            <xsl:number level="any" count="wix:Directory | wix:DirectoryRef"/>
        </xsl:variable>
        <xsl:variable name="guid">
            <xsl:value-of select="//wix:Component[@Guid][position()=$guid_offset]/@Guid"/>
        </xsl:variable>

        <xsl:copy>
            <xsl:copy-of select="@*"/>
            <Component>
                <xsl:attribute name="Guid"><xsl:value-of select="$guid" /></xsl:attribute>
                <xsl:attribute name="Id">
                    <xsl:choose>
                        <xsl:when test="$parent_path">
                            <xsl:value-of select="concat('c_', @Id)" />
                        </xsl:when>
                        <xsl:otherwise>
                            <xsl:value-of select="'c_VLC'"/>
                        </xsl:otherwise>
                    </xsl:choose>
                </xsl:attribute>
                <xsl:if test="$parent_path">
                    <RemoveFolder On="uninstall">
                        <xsl:attribute name="Id">
                            <xsl:value-of select="concat('u_', @Id)" />
                        </xsl:attribute>
                    </RemoveFolder>
                </xsl:if>
                <RegistryValue>
                    <xsl:attribute name="Root">HKCU</xsl:attribute>
                    <xsl:attribute name="Key">
                        <xsl:value-of select="$full_path" />
                    </xsl:attribute>
                    <xsl:attribute name="Name">VlcUninstall</xsl:attribute>
                    <xsl:attribute name="Type">string</xsl:attribute>
                    <xsl:attribute name="Value">${VLC_VERSION}</xsl:attribute>
                    <xsl:attribute name="KeyPath">yes</xsl:attribute>
                </RegistryValue>
                <xsl:apply-templates select="wix:Component/wix:File"/>
            </Component>
            <xsl:apply-templates select="wix:Directory">
                <xsl:with-param name="parent_path" select="$full_path"/>
            </xsl:apply-templates>
        </xsl:copy>
    </xsl:template>

    <xsl:template match="wix:Component">
        <xsl:apply-templates />
    </xsl:template>

    <xsl:template match="wix:File">
        <xsl:copy>
            <xsl:copy-of select="@Id" />
            <xsl:copy-of select="@Source" />
        </xsl:copy>
    </xsl:template>

    <xsl:template match="wix:Wix/wix:Fragment[2]">
    </xsl:template>

</xsl:stylesheet>
