--����������������10005
--����1�����è[3010006]�Ի������񱳰������Ʒ��������[2610018]

--NPC�Ի�����
function x10005_OnNPCTalk(mapID, instanceID, questID, ownerID, npcid, npctypeid)
    --����Ƿ�Ϊ��è
	if npctypeid == 3010006 then
		role.QuestAddRoleItem(mapID, instanceID, ownerID, questID, 2610018, 1, 0, 102)
	end
end

function Bqc10005 (MapID, InstanceID, QuestID, RoleID, NPCID)	--���������񲥷ŷ������㲥
	local MsgID = msg.BeginMsgEvent();
	msg.AddMsgEvent(MsgID,102,100029);
	msg.AddMsgEvent(MsgID,2,RoleID);
	msg.DispatchBroadcast(MsgID,3017298127--[[����]],InstanceID,-1);
end

--ע��
aux.RegisterQuestEvent(10005, 6, "x10005_OnNPCTalk")
aux.RegisterQuestEvent(10005, 1, "Bqc10005"); --���������� ��������������[10005]��������è����
