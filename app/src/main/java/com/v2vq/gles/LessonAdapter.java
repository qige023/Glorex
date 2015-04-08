package com.v2vq.gles;

import android.content.Context;
import android.graphics.Color;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

public class LessonAdapter extends BaseAdapter{
    private Context mContext;
    private String[] lessonNames;
    private String[] libraryNames;

    public LessonAdapter(Context context) {
        this.mContext = context;
        this.lessonNames = this.mContext.getResources().getStringArray(R.array.lesson_names);
        this.libraryNames = this.mContext.getResources().getStringArray(R.array.library_names);
    }

    @Override
    public int getCount() {
        if (this.lessonNames != null)
            return this.lessonNames.length;
        return 0;
    }

    @Override
    public Object getItem(int paramInt) {
        if (paramInt < this.lessonNames.length)
            return this.lessonNames[paramInt];
        return null;
    }

    @Override
    public long getItemId(int paramInt) {
        return 0L;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        if (convertView == null) {
            convertView = LayoutInflater.from(mContext).inflate(android.R.layout.simple_list_item_1, null);
            TextView tvTitle = (TextView) convertView.findViewById(android.R.id.text1);
            tvTitle.setTextColor(Color.BLACK);
            tvTitle.setText(this.lessonNames[position]);
        }
        return convertView;
    }

    public String getLibraryNameByPosition(int position) {
        return this.libraryNames[position];
    }
}
