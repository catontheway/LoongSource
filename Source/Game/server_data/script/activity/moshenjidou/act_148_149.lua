
Jidou = {Singlewaitinglist = {},Teamwaitinglist = {},NPCID={},RoomIndex=0,CreBuff={},RoomInfo={}}   --Singlewaitinglist�����Ŷӵ�����б�Teamwaitinglist�����Ŷӵ�����б�NPCID��õ���NPCID��RoomIndex�Ѿ������ķ���������RoomInfoÿ���������Ϣ,CreBuff�Ǵ������Ϲ�����buff�Ķ�Ӧ��
Jidou.CreBuff[4902119] = 1140301
Jidou.CreBuff[4902120] = 1140401
Jidou.CreBuff[4902121] = 1140501
Jidou.CreBuff[4902122] = 1140601

function moshen_OnStart(actID)

	--Jidou.NPCid[1] = map.MapCreateCreature(3017298383, -1, 4900801, 882, 10000, 1994)--��Ҫ3����ͬ�����꣬����ֻд��һ��������������Ҫ����
	--Jidou.NPCid[2] = map.MapCreateCreature(3017298383, -1, 4900801, , 10000, )
	--Jidou.NPCid[3] = map.MapCreateCreature(3017298383, -1, 4900801, , 10000, )

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 12928)	--ħ�񼤶���ѿ�ʼ
	msg.DispatchWorldMsgEvent(MsgID)

end

aux.RegisterActEvent(161, 2, "moshen_OnStart")
aux.RegisterActEvent(162, 2, "moshen_OnStart")

function moshen_OnEnd(actID)

	--map.MapDeleteCreature(3017298383, -1, Jidou.NPCid[1])
	--map.MapDeleteCreature(3017298383, -1, Jidou.NPCid[2])
	--map.MapDeleteCreature(3017298383, -1, Jidou.NPCid[3])

	if #Jidou.Singlewaitinglist > 0 then
		for i=1, #Jidou.Singlewaitinglist do
            unit.CancelBuff(MapID, InstanceID,Jidou.Singlewaitinglist[i], 1140901)  --ȡ��ħ���Ŷ�״̬
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 71, 3725)			--�ʱ�䵽�����ղŵı�����ȡ��
			msg.DispatchRoleMsgEvent(Jidou.Singlewaitinglist[i], MsgID)
		end
		Jidou.Singlewaitinglist = {}
	end

	if #Jidou.Teamwaitinglist > 0 then
		for i=1, #Jidou.Teamwaitinglist do
		    unit.CancelBuff(MapID, InstanceID,Jidou.Teamwaitinglist[i], 1140901)  --ȡ��ħ���Ŷ�״̬
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 71, 3725)			--�ʱ�䵽�����ղŵı�����ȡ��
			msg.DispatchRoleMsgEvent(Jidou.Teamwaitinglist[i], MsgID)
		end
		Jidou.Teamwaitinglist = {}
	end

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 12929)	--����ħ�񼤶���Ѿ�����
	msg.DispatchWorldMsgEvent(MsgID)

end

aux.RegisterActEvent(161, 3, "moshen_OnEnd")
aux.RegisterActEvent(162, 3, "moshen_OnEnd")
