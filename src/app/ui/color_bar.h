// Aseprite
// Copyright (C) 2018-2020  Igara Studio S.A.
// Copyright (C) 2001-2018  David Capello
//
// This program is distributed under the terms of
// the End-User License Agreement for Aseprite.

#ifndef APP_UI_COLOR_BAR_H_INCLUDED
#define APP_UI_COLOR_BAR_H_INCLUDED
#pragma once

#include "app/color.h"
#include "app/context_observer.h"
#include "app/doc_observer.h"
#include "app/docs_observer.h"
#include "app/tilemap_mode.h"
#include "app/tileset_mode.h"
#include "app/ui/button_set.h"
#include "app/ui/color_button.h"
#include "app/ui/input_chain_element.h"
#include "app/ui/palette_view.h"
#include "doc/object_id.h"
#include "doc/palette_gradient_type.h"
#include "doc/pixel_format.h"
#include "doc/sort_palette.h"
#include "doc/tileset.h"
#include "obs/connection.h"
#include "obs/signal.h"
#include "ui/box.h"
#include "ui/button.h"
#include "ui/splitter.h"
#include "ui/view.h"

namespace ui {
  class TooltipManager;
}

namespace app {
  class ColorButton;
  class ColorSpectrum;
  class ColorTintShadeTone;
  class ColorWheel;
  class CommandExecutionEvent;
  class PaletteIndexChangeEvent;
  class PalettePopup;
  class PalettesLoader;

  class ColorBar : public ui::Box
                 , public PaletteViewDelegate
                 , public ContextObserver
                 , public DocObserver
                 , public InputChainElement {
    static ColorBar* m_instance;
  public:
    enum class ColorSelector {
      NONE,
      SPECTRUM,
      RGB_WHEEL,
      RYB_WHEEL,
      TINT_SHADE_TONE,
      NORMAL_MAP_WHEEL,
    };

    static ColorBar* instance() { return m_instance; }

    ColorBar(int align, ui::TooltipManager* tooltipManager);
    ~ColorBar();

    void setPixelFormat(PixelFormat pixelFormat);

    app::Color getFgColor() const;
    app::Color getBgColor() const;
    void setFgColor(const app::Color& color);
    void setBgColor(const app::Color& color);

    doc::tile_index getFgTile() const;
    doc::tile_index getBgTile() const;

    PaletteView* getPaletteView() { return &m_paletteView; }
    PaletteView* getTilesView() { return &m_tilesView; }

    ColorSelector getColorSelector() const;
    void setColorSelector(ColorSelector selector);

    // Used by the Palette Editor command to change the status of button
    // when the visibility of the dialog changes.
    bool inEditMode() const;
    void setEditMode(bool state);

    TilemapMode tilemapMode() const;
    void setTilemapMode(const TilemapMode mode);

    TilesetMode tilesetMode() const;
    void setTilesetMode(const TilesetMode mode);

    ColorButton* fgColorButton() { return &m_fgColor; }
    ColorButton* bgColorButton() { return &m_bgColor; }

    // ContextObserver impl
    void onActiveSiteChange(const Site& site) override;

    // DocObserver impl
    void onGeneralUpdate(DocEvent& ev) override;
    void onTilesetChanged(DocEvent& ev) override;

    // InputChainElement impl
    void onNewInputPriority(InputChainElement* element,
                            const ui::Message* msg) override;
    bool onCanCut(Context* ctx) override;
    bool onCanCopy(Context* ctx) override;
    bool onCanPaste(Context* ctx) override;
    bool onCanClear(Context* ctx) override;
    bool onCut(Context* ctx) override;
    bool onCopy(Context* ctx) override;
    bool onPaste(Context* ctx) override;
    bool onClear(Context* ctx) override;
    void onCancel(Context* ctx) override;

    obs::signal<void()> ChangeSelection;

  protected:
    void onAppPaletteChange();
    void onFocusPaletteOrTilesView(ui::Message* msg);
    void onBeforeExecuteCommand(CommandExecutionEvent& ev);
    void onAfterExecuteCommand(CommandExecutionEvent& ev);
    void onSwitchPalEditMode();
    void onPaletteButtonClick();
    void onTilesButtonClick();
    void onTilesetModeButtonClick();
    void onTilesetOptionsClick();
    void onRemapPalButtonClick();
    void onRemapTilesButtonClick();
    void onPaletteIndexChange(PaletteIndexChangeEvent& ev);
    void onFgColorChangeFromPreferences();
    void onBgColorChangeFromPreferences();
    void onFgColorButtonBeforeChange(app::Color& color);
    void onFgColorButtonChange(const app::Color& color);
    void onBgColorButtonChange(const app::Color& color);
    void onColorButtonChange(const app::Color& color);
    void onPickSpectrum(const app::Color& color, ui::MouseButton button);
    void onReverseColors();
    void onSortBy(doc::SortPaletteBy channel);
    void onGradient(GradientType gradientType);
    void onFixWarningClick(ColorButton* colorButton, ui::Button* warningIcon);
    void onTimerTick();
    void setAscending(bool ascending);

    // PaletteViewDelegate impl
    void onPaletteViewIndexChange(int index, ui::MouseButton button) override;
    void onPaletteViewModification(const doc::Palette* newPalette, PaletteViewModification mod) override;
    void onPaletteViewChangeSize(int boxsize) override;
    void onPaletteViewPasteColors(const Palette* fromPal, const doc::PalettePicks& from, const doc::PalettePicks& to) override;
    app::Color onPaletteViewGetForegroundIndex() override;
    app::Color onPaletteViewGetBackgroundIndex() override;
    void onTilesViewClearTiles(const doc::PalettePicks& picks) override;
    void onTilesViewResize(const int newSize) override;
    void onTilesViewDragAndDrop(doc::Tileset* tileset,
                                doc::PalettePicks& picks,
                                int& currentEntry,
                                const int beforeIndex,
                                const bool isCopy) override;
    void onTilesViewIndexChange(int index, ui::MouseButton button) override;

  private:
    void showRemapPal();
    void showRemapTiles();
    void hideRemapPal();
    void hideRemapTiles();
    void setPalette(const doc::Palette* newPalette, const std::string& actionText);
    void setTransparentIndex(int index);
    void updateWarningIcon(const app::Color& color, ui::Button* warningIcon);
    int setPaletteEntry(const app::Color& color);
    void updateCurrentSpritePalette(const char* operationName);
    void setupTooltips(ui::TooltipManager* tooltipManager);
    void registerCommands();
    void showPaletteSortOptions();
    void showPalettePresets();
    void showPaletteOptions();
    static void fixColorIndex(ColorButton& color);

    class ScrollableView : public ui::View {
    public:
      ScrollableView();
    protected:
      void onInitTheme(ui::InitThemeEvent& ev) override;
    };

    class WarningIcon;

    ui::HBox m_palHBox;
    ui::HBox m_tilesHBox;
    ButtonSet m_editPal;
    ButtonSet m_buttons;
    ButtonSet m_tilesButton;
    ButtonSet m_tilesetModeButtons;
    std::unique_ptr<PalettePopup> m_palettePopup;
    ui::Splitter m_splitter;
    ui::VBox m_palettePlaceholder;
    ui::VBox m_selectorPlaceholder;
    ScrollableView m_scrollablePalView;
    ScrollableView m_scrollableTilesView;
    PaletteView m_paletteView;
    PaletteView m_tilesView;
    ui::Button m_remapPalButton;
    ui::Button m_remapTilesButton;
    ColorSelector m_selector;
    ColorTintShadeTone* m_tintShadeTone;
    ColorSpectrum* m_spectrum;
    ColorWheel* m_wheel;
    ColorButton m_fgColor;
    ColorButton m_bgColor;
    WarningIcon* m_fgWarningIcon;
    WarningIcon* m_bgWarningIcon;

    // True when the user clicks the PaletteView so we're changing the
    // color from the palette view.
    bool m_fromPalView;

    // If m_syncingWithPref is true it means that the eyedropper was
    // used to change the color.
    bool m_fromPref;

    bool m_fromFgButton;
    bool m_fromBgButton;

    std::unique_ptr<doc::Palette> m_oldPalette;
    std::unique_ptr<doc::Tileset> m_oldTileset;
    Doc* m_lastDocument;
    doc::ObjectId m_lastTilesetId;
    bool m_ascending;
    obs::scoped_connection m_beforeCmdConn;
    obs::scoped_connection m_afterCmdConn;
    obs::scoped_connection m_fgConn;
    obs::scoped_connection m_bgConn;
    obs::scoped_connection m_sepConn;
    obs::scoped_connection m_appPalChangeConn;
    ui::MouseButton m_lastButton;

    // True if the editing mode is on.
    bool m_editMode;

    // True if we should be putting/setting tiles.
    TilemapMode m_tilemapMode;
    TilesetMode m_tilesetMode;

    // Timer to redraw editors after a palette change.
    ui::Timer m_redrawTimer;
    bool m_redrawAll;

    // True if a palette change must be implant in the UndoHistory
    // (e.g. when two or more changes in the palette are made in a
    // very short time).
    bool m_implantChange;

    // True if the App::PaletteChange signal is generated by this same
    // ColorBar.
    bool m_selfPalChange;
  };

  class DisableColorBarEditMode {
  public:
    DisableColorBarEditMode()
      : m_colorBar(ColorBar::instance())
      , m_oldMode(m_colorBar->inEditMode()) {
      if (m_oldMode)
        m_colorBar->setEditMode(false);
    }
    ~DisableColorBarEditMode() {
      if (m_oldMode)
        m_colorBar->setEditMode(true);
    }
  private:
    ColorBar* m_colorBar;
    bool m_oldMode;
  };

} // namespace app

#endif
