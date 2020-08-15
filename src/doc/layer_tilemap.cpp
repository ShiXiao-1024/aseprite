// Aseprite Document Library
// Copyright (c) 2019  Igara Studio S.A.
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "doc/layer_tilemap.h"

#include "doc/primitives.h"
#include "doc/sprite.h"
#include "doc/tilesets.h"

namespace doc {

LayerTilemap::LayerTilemap(Sprite* sprite, const tileset_index tsi)
  : LayerImage(ObjectType::LayerTilemap, sprite)
  , m_tileset(sprite->tilesets()->get(tsi))
  , m_tilesetIndex(tsi)
{
  ASSERT(m_tileset);
}

LayerTilemap::~LayerTilemap()
{
}

void LayerTilemap::setTilesetIndex(tileset_index tsi)
{
  m_tilesetIndex = tsi;
  m_tileset = sprite()->tilesets()->get(tsi);
}

} // namespace doc
