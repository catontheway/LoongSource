--�һ������������
function x20004_Oncomplete(MapID, InstanceID, QuestID, OwnerID, NPCID)
	if Act4_Forage < 50 then
		--�ۼ��ռ��ľ�������
		Act4_Forage = Act4_Forage + 1
		--����ǰ�ռ����������Ϊ##����ǰ�ռ�������/50�����������ȫ����ҿɻ�ý�����
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 12)
		msg.AddMsgEvent(MsgID, 12, Act4_Forage)
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)

		if Act4_Forage == 50 then
		    --ȫ��ͨ�棺�����շ�������У��ɹ��ӵз�����������ʣ�ȫ���Ա�ɻ�ö��⽱������
		    local MsgID = msg.BeginMsgEvent()
		    msg.AddMsgEvent(MsgID, 100, 13)
		    msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)

		    --�Զ����������ߵ�������һ������������״̬
		    act.AddAllRoleBuff(2011901)
	    end
	end
end

--aux.RegisterQuestEvent(20004, 1, "x20004_Oncomplete")
