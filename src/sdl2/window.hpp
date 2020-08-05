#pragma once

#include <SDL_syswm.h>
#include <SDL_video.h>

#include <utility>
#include <optional>
#include <string>

namespace sdl2
{
	using WindowView = SDL_Window*;

	enum class WindowFlags : std::uint32_t
	{
		FULLSCREEN = SDL_WINDOW_FULLSCREEN,
		OPENGL = SDL_WINDOW_OPENGL,
		SHOWN = SDL_WINDOW_SHOWN,
		HIDDEN = SDL_WINDOW_HIDDEN,
		BORDERLESS = SDL_WINDOW_BORDERLESS,
		RESIZABLE = SDL_WINDOW_RESIZABLE,
		MINIMIZED = SDL_WINDOW_MINIMIZED,
		MAXIMIZED = SDL_WINDOW_MAXIMIZED,
		GRABBED = SDL_WINDOW_INPUT_GRABBED,
		INPUT_FOCUS = SDL_WINDOW_INPUT_FOCUS,
		MOUSE_FOCUS = SDL_WINDOW_MOUSE_FOCUS,
		FULLSCREEN_DESKTOP = SDL_WINDOW_FULLSCREEN_DESKTOP,
		FOREIGN = SDL_WINDOW_FOREIGN,
		ALLOW_HIGHDPI = SDL_WINDOW_ALLOW_HIGHDPI,
		MOUSE_CAPTURE = SDL_WINDOW_MOUSE_CAPTURE,
		ALWAYS_ON_TOP = SDL_WINDOW_ALWAYS_ON_TOP,
		SKIP_TASKBAR = SDL_WINDOW_SKIP_TASKBAR,
		UTILITY = SDL_WINDOW_UTILITY,
		TOOLTIP = SDL_WINDOW_TOOLTIP,
		POPUP_MENU = SDL_WINDOW_POPUP_MENU,
		VULKAN = SDL_WINDOW_VULKAN
	};

	constexpr inline WindowFlags operator|(WindowFlags a, WindowFlags b)noexcept
	{
		return static_cast<WindowFlags>(static_cast<std::uint32_t>(a) | static_cast<std::uint32_t>(b));
	}

	class Window
	{
	public:
		[[nodiscard]] constexpr explicit Window(WindowView view)noexcept
			: m_Window(view)
		{};

		[[nodiscard]] constexpr Window()noexcept = default;

		[[nodiscard]] Window(const std::string& title, int x, int y, int w, int h, WindowFlags flags)noexcept
			: m_Window(SDL_CreateWindow(title.c_str(), x, y, w, h, static_cast<std::uint32_t>(flags)))
		{}

		[[nodiscard]] explicit Window(const void* driverDependentWindowData)noexcept : m_Window(SDL_CreateWindowFrom(driverDependentWindowData)) {}

		~Window()noexcept
		{
			if (m_Window)
			{
				SDL_DestroyWindow(m_Window);
			}
		}

		Window(Window&)noexcept = delete;
		[[nodiscard]] Window(Window&& w)noexcept : m_Window(std::move(w.m_Window)) { w.m_Window = nullptr; }

		Window& operator=(Window&)noexcept = delete;
		[[nodiscard]] Window& operator=(Window&& w)noexcept 
		{
			if (m_Window != w.m_Window)
			{
				SDL_DestroyWindow(m_Window);
				m_Window = w.m_Window;
			}
			w.m_Window = nullptr;
			return *this;
		}

		[[nodiscard]] int getDisplayIndex() const noexcept { return SDL_GetWindowDisplayIndex(m_Window); }

		[[nodiscard]] std::optional<SDL_DisplayMode> getDisplayMode() const noexcept 
		{
			SDL_DisplayMode mode;
			if (SDL_GetWindowDisplayMode(m_Window, &mode) != 0)
			{
				return std::nullopt;
			}
			return mode;
		}

		bool setDisplayMode(SDL_DisplayMode const& mode) const noexcept { return SDL_SetWindowDisplayMode(m_Window, &mode) == 0;	}

		[[nodiscard]] Uint32 getFlags() const noexcept { return SDL_GetWindowFlags(m_Window); }

		static WindowView GetGrabbedWindow() noexcept { return SDL_GetGrabbedWindow(); }

		[[nodiscard]] bool isGrabbed() const noexcept { return SDL_GetWindowGrab(m_Window); }

		void setGrabbed(bool grabbed) noexcept { SDL_SetWindowGrab(m_Window, static_cast<SDL_bool>(grabbed)); }

		[[nodiscard]] SDL_Point getPosition()const noexcept 
		{
			SDL_Point p;
			SDL_GetWindowPosition(m_Window, &p.x, &p.y);
			return p;
		}

		void setPosition(int x, int y) noexcept { SDL_SetWindowPosition(m_Window, x, y); }

		[[nodiscard]] SDL_Point getSize()const noexcept
		{
			SDL_Point p;
			SDL_GetWindowSize(m_Window, &p.x, &p.y);
			return p;
		}

		void setSize(int w, int h) const { SDL_SetWindowSize(m_Window, w, h); }

		[[nodiscard]] SDL_Point getMinimumSize()const noexcept
		{
			SDL_Point p;
			SDL_GetWindowMinimumSize(m_Window, &p.x, &p.y);
			return p;
		}

		void setMinimumSize(int w, int h) const { SDL_SetWindowMinimumSize(m_Window, w, h); }

		[[nodiscard]] SDL_Point getMaximumSize()const noexcept
		{
			SDL_Point p;
			SDL_GetWindowMaximumSize(m_Window, &p.x, &p.y);
			return p;
		}

		void setMaximumSize(int w, int h) const { SDL_SetWindowMaximumSize(m_Window, w, h); }

		void setBordered(bool bordered) const { SDL_SetWindowBordered(m_Window, static_cast<SDL_bool>(bordered)); }

		void seResizable(bool resizable) const { SDL_SetWindowResizable(m_Window, static_cast<SDL_bool>(resizable)); }

		void setTitle(std::string const& title) noexcept {	SDL_SetWindowTitle(m_Window, title.c_str()); }

		[[nodiscard]]  std::string getTitle()const noexcept { return std::string{ SDL_GetWindowTitle(m_Window) }; }

		void show() noexcept { SDL_ShowWindow(m_Window); }

		void hide() noexcept { SDL_HideWindow(m_Window); }

		void maximize()	noexcept { SDL_MaximizeWindow(m_Window); }

		void minimize()	noexcept { SDL_MinimizeWindow(m_Window); }

		void raise() noexcept { SDL_RaiseWindow(m_Window); }

		void restore() noexcept { SDL_RestoreWindow(m_Window); }

		bool isFullscreen() const noexcept { return getFlags() & (SDL_WINDOW_FULLSCREEN | SDL_WINDOW_FULLSCREEN_DESKTOP); }

		bool setFullscreen(bool fs) noexcept { return SDL_SetWindowFullscreen(m_Window, fs ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0) == 0;  }

		[[nodiscard]] float getBrightness() const noexcept { return SDL_GetWindowBrightness(m_Window); }

		bool setBrightness(float brightness) noexcept { return SDL_SetWindowBrightness(m_Window, brightness) == 0; }

		[[nodiscard]] std::optional<float> getOpacity() const noexcept 
		{
			float opacity;
			if (SDL_GetWindowOpacity(m_Window, &opacity) != 0)
			{
				return opacity;
			}
			return std::nullopt; 
		}

		bool setOpacity(float opacity) noexcept { return SDL_SetWindowOpacity(m_Window, opacity) == 0; }

		bool setParent(Window& childWindow) { return SDL_SetWindowModalFor(m_Window, childWindow.get()) == 0; }

		bool setInputFocus() { return SDL_SetWindowInputFocus(m_Window) == 0; }

		[[nodiscard]] static WindowView findById(std::uint32_t id) { return SDL_GetWindowFromID(id); }

		[[nodiscard]] std::uint32_t getId()const { return SDL_GetWindowID(m_Window); }

		[[nodiscard]] void* getData(const std::string& name) { return SDL_GetWindowData(m_Window, name.c_str()); }

		void setData(const std::string& name, void* userdata) { SDL_SetWindowData(m_Window, name.c_str(), userdata); }

		bool updateSurface() { return SDL_UpdateWindowSurface(m_Window) == 0; }

		bool updateSurface(const SDL_Rect* rects, int numrects) { return SDL_UpdateWindowSurfaceRects(m_Window, rects, numrects) == 0; }

		[[nodiscard]] std::optional<SDL_SysWMinfo> getWindowManagerInfo() const noexcept
		{
			SDL_SysWMinfo info;
			SDL_VERSION(&info.version);
			if (SDL_GetWindowWMInfo(m_Window, &info) == SDL_FALSE)
			{
				return std::nullopt;
			}
			return info;
		}

		[[nodiscard]] bool isValid()const noexcept { return m_Window != nullptr; }

		[[nodiscard]] WindowView get()const noexcept { return m_Window; }

		void glSwap()noexcept { SDL_GL_SwapWindow(m_Window); }

		[[nodiscard]] SDL_GLContext createGLContext()noexcept { return SDL_GL_CreateContext(m_Window); }

		bool makeCurrent(SDL_GLContext context)noexcept { return SDL_GL_MakeCurrent(m_Window, context) == 0; }

	private:
		SDL_Window* m_Window = nullptr;
	};
}
