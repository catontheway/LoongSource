--���ﺰ���ű�

-- �������ս������
function c1003169_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 65)
       end	
end

aux.RegisterCreatureEvent(1003169, 2, "c1003169_OnEnterCombat")

