using System.Net.WebSockets;
using System.Text;

namespace Client.Web.WebSockets
{
    public class Sockets
    {
        private ClientWebSocket _webSocket;

        public Sockets()
        {
            _webSocket = new ClientWebSocket();
        }

        public async Task ConnectAsync(Uri uri, CancellationToken cancellationToken)
        {
            await _webSocket.ConnectAsync(uri, cancellationToken);
        }

        public async Task SendMessageAsync(string message, CancellationToken cancellationToken)
        {
            if (_webSocket.State == WebSocketState.Open)
            {
                var buffer = new ArraySegment<byte>(Encoding.UTF8.GetBytes(message));
                await _webSocket.SendAsync(buffer, WebSocketMessageType.Text, true, cancellationToken);
            }
        }

        public async Task<string> ReceiveMessageAsync(CancellationToken cancellationToken)
        {
            var buffer = new ArraySegment<byte>(new byte[4096]);
            var result = await _webSocket.ReceiveAsync(buffer, cancellationToken);

            if (result.MessageType == WebSocketMessageType.Text)
            {
                return Encoding.UTF8.GetString(buffer.Array, 0, result.Count);
            }

            return null;
        }

        public async Task CloseAsync(WebSocketCloseStatus closeStatus, string statusDescription, CancellationToken cancellationToken)
        {
            await _webSocket.CloseAsync(closeStatus, statusDescription, cancellationToken);
            _webSocket.Dispose();
        }
    }
}
