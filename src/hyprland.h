#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

enum HyprlandSocketType { REQUESTS_SOCKET, EVENT_SOCKET };

int hyprlandConnect(enum HyprlandSocketType type) {
  const char *runtime_dir = getenv("XDG_RUNTIME_DIR");
  const char *his = getenv("HYPRLAND_INSTANCE_SIGNATURE");

  char path[256];
  if (type == EVENT_SOCKET) {
    snprintf(path, sizeof(path), "%s/hypr/%s/.socket2.sock", runtime_dir, his);
  } else {
    snprintf(path, sizeof(path), "%s/hypr/%s/.socket.sock", runtime_dir, his);
  }

  int sock = socket(AF_UNIX, SOCK_STREAM, 0);
  struct sockaddr_un addr = {.sun_family = AF_UNIX};
  strncpy(addr.sun_path, path, sizeof(addr.sun_path) - 1);

  if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
    perror("hyprland socket connection failed");
    return -1;
  }

  return sock;
}

void hyprlandDispatch(const char *cmd) {
  int sock = hyprlandConnect(REQUESTS_SOCKET);
  printf("%s\n", cmd);
  write(sock, cmd, strlen(cmd));
  close(sock);
}


