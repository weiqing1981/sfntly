package com.google.typography.font.sfntly.table.opentype.ligaturesubst;

import com.google.typography.font.sfntly.data.ReadableFontData;
import com.google.typography.font.sfntly.table.opentype.component.OffsetRecordTable;
import com.google.typography.font.sfntly.table.opentype.component.VisibleBuilder;

public class LigatureSet extends OffsetRecordTable<Ligature> {
  public LigatureSet(ReadableFontData data, int base, boolean dataIsCanonical) {
    super(data, base, dataIsCanonical);
  }

  public static class Builder extends OffsetRecordTable.Builder<LigatureSet, Ligature> {

    public Builder(ReadableFontData data, boolean dataIsCanonical) {
      super(data, dataIsCanonical);
    }

    public Builder() {
      super();
    }

    public Builder(LigatureSet table) {
      super(table);
    }

    @Override
    protected LigatureSet readTable(ReadableFontData data, int base, boolean dataIsCanonical) {
      return new LigatureSet(data, base, dataIsCanonical);
    }

    @Override
    protected VisibleBuilder<Ligature> createSubTableBuilder() {
      return new Ligature.Builder();
    }

    @Override
    protected VisibleBuilder<Ligature> createSubTableBuilder(
        ReadableFontData data, boolean dataIsCanonical) {
      return new Ligature.Builder(data, dataIsCanonical);
    }

    @Override
    protected VisibleBuilder<Ligature> createSubTableBuilder(Ligature subTable) {
      return new Ligature.Builder(subTable);
    }

    @Override
    protected void initFields() {
    }

    @Override
    public int fieldCount() {
      return 0;
    }
  }

  @Override
  protected Ligature readSubTable(ReadableFontData data, boolean dataIsCanonical) {
    return new Ligature(data, base, dataIsCanonical);
  }

  @Override
  public int fieldCount() {
    return 0;
  }
}
