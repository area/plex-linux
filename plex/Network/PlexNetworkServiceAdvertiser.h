/*
 *  Copyright (C) 2011 Plex, Inc.
 *
 *  Created on: Apr 10, 2011
 *      Author: Elan Feingold
 */

#pragma once

#include "GUISettings.h"
#include "NetworkServiceAdvertiser.h"
#include "CocoaUtils.h"
#include "PlexUtils.h"

/////////////////////////////////////////////////////////////////////////////
class PlexNetworkServiceAdvertiser : public NetworkServiceAdvertiser
{
 public:
  
  /// Constructor.
  PlexNetworkServiceAdvertiser(boost::asio::io_service& ioService)
    : NetworkServiceAdvertiser(ioService, NS_BROADCAST_ADDR, NS_PLEX_MEDIA_CLIENT_PORT) {}
  
 protected:
  
  /// For subclasses to fill in.
  virtual void createReply(map<string, string>& headers) 
  {
    headers["Name"] = GetHostName();
    headers["Port"] = g_guiSettings.GetString("services.webserverport");
#if !defined(_WIN32) && !defined(_LINUX)
    headers["Version"] = Cocoa_GetAppVersion();
#endif
    headers["Product"] = "Plex/Nine (dharma)";
  }
  
  /// For subclasses to fill in.
  virtual string getType()
  {
    return "plex/media-player";
  }
  
  virtual string getResourceIdentifier() { return g_guiSettings.GetString("system.uuid");  }
  virtual string getBody() { return ""; }
};
