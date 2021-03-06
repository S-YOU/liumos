#include "corefunc.h"
#include "liumos.h"

Sheet* screen_sheet;

Sheet vram_sheet_;
Sheet screen_sheet_;

void InitGraphics() {
  const EFI::GraphicsOutputProtocol::ModeInfo& mode =
      CoreFunc::GetEFI().GetGraphicsModeInfo();
  vram_sheet_.Init(static_cast<uint8_t*>(

                       mode.frame_buffer_base),
                   mode.info->horizontal_resolution,
                   mode.info->vertical_resolution,
                   mode.info->pixels_per_scan_line);
  liumos->vram_sheet = &vram_sheet_;
  screen_sheet = &vram_sheet_;
  liumos->screen_sheet = screen_sheet;
}

void InitDoubleBuffer() {
  screen_sheet_.Init(
      liumos->dram_allocator->AllocPages<uint8_t*>(
          (vram_sheet_.GetBufSize() + kPageSize - 1) >> kPageSizeExponent),
      vram_sheet_.GetXSize(), vram_sheet_.GetYSize(),
      vram_sheet_.GetPixelsPerScanLine());
  screen_sheet_.SetParent(&vram_sheet_);
  memcpy(screen_sheet_.GetBuf(), vram_sheet_.GetBuf(),
         screen_sheet_.GetBufSize());
  screen_sheet = &screen_sheet_;
  liumos->screen_sheet = screen_sheet;
  screen_sheet->Flush(0, 0, screen_sheet->GetXSize(), screen_sheet->GetYSize());
}
