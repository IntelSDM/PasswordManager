using System.Net.WebSockets;
using System.Text;

namespace Client.Web
{
    public class Sockets
    {
        private ClientWebSocket Socket;

        public Sockets()
        {
            Socket = new ClientWebSocket();
        }

        public async Task ConnectAsync(Uri uri, CancellationToken cancellationtoken)
        {
            await Socket.ConnectAsync(uri, cancellationtoken);
        }

        public async Task SendMessageAsync(string message, CancellationToken cancellationtoken)
        {
            if (Socket.State == WebSocketState.Open)
            {
                var buffer = new ArraySegment<byte>(Encoding.UTF8.GetBytes(message));
                await Socket.SendAsync(buffer, WebSocketMessageType.Text, true, cancellationtoken);
            }
        }

        public async Task<string> ReceiveMessageAsync(CancellationToken cancellationtoken)
        {
            var buffer = new ArraySegment<byte>(new byte[4096]);
            var result = await Socket.ReceiveAsync(buffer, cancellationtoken);

            if (result.MessageType == WebSocketMessageType.Text)
            {
                return Encoding.UTF8.GetString(buffer.Array, 0, result.Count);
            }

            return null;
        }

        public async Task CloseAsync(WebSocketCloseStatus closestatus, string statusdescription, CancellationToken cancellationtoken)
        {
            await Socket.CloseAsync(closestatus, statusdescription, cancellationtoken);
            Socket.Dispose();
        }
    }
}
