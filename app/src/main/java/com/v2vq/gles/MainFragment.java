package com.v2vq.gles;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ListView;

/**
 * A placeholder fragment containing a simple view.
 */
public class MainFragment extends Fragment {

    private Context mContext;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        mContext = getActivity().getApplicationContext();
        View rootView = inflater.inflate(R.layout.fragment_main, container, false);
        ListView lvLessons = (ListView) rootView.findViewById(R.id.lv_lessons);
        final LessonAdapter lessonAdapter = new LessonAdapter(mContext);
        lvLessons.setAdapter(lessonAdapter);
        lvLessons.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                String libraryName = lessonAdapter.getLibraryNameByPosition(position);
                libraryName = mContext.getPackageName() + ".stubs." + libraryName;
                try {
                    Class<?> cls =  Class.forName(libraryName);
                    Intent intent = new Intent(mContext, cls);
                    getActivity().startActivity(intent);
                    getActivity().overridePendingTransition(R.anim.right2left_enter, R.anim.right2left_exit);
                } catch (ClassNotFoundException exception) {
                    exception.printStackTrace();
                }

            }
        });
        return rootView;
    }
}
