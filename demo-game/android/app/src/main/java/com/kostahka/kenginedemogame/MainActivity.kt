package com.kostahka.kenginedemogame

import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.tooling.preview.Preview
import org.libsdl.app.SDLActivity

class MainActivity : SDLActivity() {
    override fun getLibraries(): Array<String>
    {
        return arrayOf<String>(
            "kengine-demo-static"
        )
    }
}