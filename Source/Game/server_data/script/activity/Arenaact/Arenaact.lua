--�������
Arenaact_min = 0
Arenaact2_min = 0
Arenaact_isopen = 0
Arenaact_times = {}
guangbo_Arena = 0
function Arenaact_OnStart(actID)
    Arenaact_isopen = 1
    Arenaact_min = 0
    Arenaact_times = {}
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100094)--��������ڼ���ҿ��Ի�ö�����ɽ�ɽ������ͬʱֻҪ���10���������������ܻ�ô����ľ��飡
	msg.DispatchBroadcast(MsgID,-1,-1,-1)

end

function Arenaact_OnTimerMin(actID)

    guangbo_Arena = 0
	Arenaact_min = Arenaact_min + 1
	if Arenaact_min == 5 or Arenaact_min == 10 or Arenaact_min == 15 or Arenaact_min == 20 then
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100094)--��������ڼ���ҿ��Ի�ö�����ɽ�ɽ������ͬʱֻҪ���10���������������ܻ�ô����ľ��飡
		msg.DispatchBroadcast(MsgID,-1,-1,-1)
	end

end

function Arenaact_OnEnd(actID)
	Arenaact_isopen = 0
	Arenaact_times = {}
end

function Arenaact2_OnStart(actID)
    Arenaact2_min = 0
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100124)--����Ľ���������Ķ��ѣ��µľ�����ģʽ�ȴ��������룬�����Ļ�ʤ�߸��л���������ʱ����װ���ϳɲ��Ͻ�����
	msg.DispatchBroadcast(MsgID,-1,-1,-1)

end

function Arenaact2_OnTimerMin(actID)

	Arenaact2_min = Arenaact2_min + 1
	if Arenaact_min == 5 or Arenaact_min == 10 or Arenaact_min == 15 or Arenaact_min == 20 then
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100124)--����Ľ���������Ķ��ѣ��µľ�����ģʽ�ȴ��������룬�����Ļ�ʤ�߸��л���������ʱ����װ���ϳɲ��Ͻ�����
		msg.DispatchBroadcast(MsgID,-1,-1,-1)
	end

end

--ע��û������¼�
aux.RegisterActEvent(118, 4, "Arenaact_OnTimerMin")
aux.RegisterActEvent(118, 2, "Arenaact_OnStart")
aux.RegisterActEvent(118, 3, "Arenaact_OnEnd")
aux.RegisterActEvent(147, 2, "Arenaact2_OnStart")
aux.RegisterActEvent(147, 4, "Arenaact2_OnTimerMin")
